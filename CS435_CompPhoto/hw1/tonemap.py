"""tonemap -- Tone map an HDR image.

Tonemap an HDR input image using a global method or local method.
"""

import argparse
import sys

import numpy as np
import scipy.misc

import Imath
import OpenEXR


def loadexr(filename):
    """Open an exr image and return a numpy array."""
    f = OpenEXR.InputFile(filename)
    dw = f.header()['dataWindow']
    sz = (dw.max.y - dw.min.y + 1, dw.max.x - dw.min.x + 1)

    # Read the three color channels as 32-bit floats
    FLOAT = Imath.PixelType(Imath.PixelType.FLOAT)
    image = np.empty((sz[0], sz[1], 3), dtype=np.float32)
    image[..., 0] = np.fromstring(f.channel("R", FLOAT), dtype=np.float32).reshape(sz)
    image[..., 1] = np.fromstring(f.channel("G", FLOAT), dtype=np.float32).reshape(sz)
    image[..., 2] = np.fromstring(f.channel("B", FLOAT), dtype=np.float32).reshape(sz)

    return image


def bilateral_filter(image, spatialsigma, rangesigma):
    """Compute bilateral filter of image"""
    dims = image.shape
    height = dims[0]
    width = dims[1]
    wx = 3
    wy = 3

    p = np.power
    a = np.absolute

    def w(i, j, k, l):
        s = ( p(i-k, 2) + p(j-l, 2) ) / ( 2*p(spatialsigma,2) )
        r = ( p( a(image[i,j] - image[k,l]), 2) ) / ( 2*p(rangesigma,2) )
        t = (-s) - r
        return np.exp(t)


    filtered = np.zeros(image.shape)
    for i in xrange(height):
        for j in xrange(width):
            Ikl = 0
            weight = 0
            pairs = []
            if i != 0:
                pairs.append((i-1, j))
            if i != height-1:
                pairs.append((i+1, j))
            if j != 0:
                pairs.append((i, j-1))
            if j != width-1:
                pairs.append((i, j+1))
            if i != 0 and j != 0:
                pairs.append((i-1, j-1))
            if i != height-1 and j != 0:
                pairs.append((i+1, j-1))
            if i != 0 and j != width-1:
                pairs.append((i-1, j+1))
            if i != height-1 and j != width-1:
                pairs.append((i+1, j+1))

            for pair in pairs:
                k = pair[0]
                l = pair[1]
                iweight = w(i, j, k, l)
                Ikl += image[k, l] * iweight
                weight += iweight
            filtered[i, j] = Ikl / weight

    return filtered


def log_tonemap(image):
    image[image == 0] = np.min(image) / 16.
    logimage = np.log(image)
    imgmin = np.min(logimage)
    imgmax = np.max(logimage)
    return (255. * (logimage - imgmin) / (imgmax - imgmin)).astype(np.uint8)


def divide_tonemap(image):
    divimage = image / (1. + image)
    return (divimage * (255. / np.max(divimage))).astype(np.uint8)


def sqrt_tonemap(image):
    sqrtimage = np.sqrt(image)
    return (sqrtimage * (255. / np.max(sqrtimage))).astype(np.uint8)


def bilateral_tonemap(image):
    """Tonemap image (HDR) using Durand 2002"""
    # compute intensity
    intensity = np.add.reduce(image, 2)
    intensity = np.divide(intensity, 3)

    # compute log intensity
    log_intensity_image = np.log(intensity)

    # compute bilateral filter
    filtered = bilateral_filter(log_intensity_image, 0.02 * image.size, 0.4)

    # compute detail layer
    detail = log_intensity_image - filtered

    # apply an offset and scale to the base
    dR = 5
    s = dR / (np.max(filtered) - np.min(filtered))
    filtered_prime = (filtered - np.max(filtered))  * s

    # reconstruct the log intensity
    o = np.power(2, (filtered_prime + detail))

    # put back colors
    (r, g, b) = o * (image[..., 0]/intensity, image[..., 1]/intensity, image[..., 2]/intensity)
    colored = np.zeros(image.shape)
    print "colors in"

    # gamma compress
    r = np.power(r, 0.5)
    g = np.power(g, 0.5)
    b = np.power(b, 0.5)
    print "gammas compressed"

    colored = np.dstack((r, g, b))
    tonemapped = colored
    print "is image"

    # rescale to 0..255 range
    tonemapped = (255 * (tonemapped - np.min(tonemapped)) ) / (np.max(tonemapped) - np.min(tonemapped))
    print "channels rescaled"

    # convert to LDR
    return tonemapped.astype(np.uint8)


if __name__ == "__main__":
    def check_method(s):
        if s == 'durand02':
            return bilateral_tonemap
        elif s == 'log':
            return log_tonemap
        elif s == 'divide':
            return divide_tonemap
        elif s == 'sqrt':
            return sqrt_tonemap
        else:
            raise argparse.ArgumentTypeError('Unknown method')

    description = '\n\n'.join(__doc__.split('\n\n'))
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('output_path', type=str)
    parser.add_argument('input_path', type=str)
    parser.add_argument('--method', type=check_method, default=bilateral_tonemap,
                        help="Must be one of, 'durand02', 'log', 'divide', or 'sqrt'")
    if len(sys.argv) == 1:
        parser.print_help()

    args = parser.parse_args()

    scipy.misc.imsave(args.output_path, args.method(loadexr(args.input_path)))

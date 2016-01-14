#!/usr/bin/python

# x, y -- x and y coordinates of point
# poly -- a list of tuples [(x, y), (x, y), ...]


def isPointInPath(x, y, poly):
	num = len(poly)
	i = 0
	j = num - 1
	c = False
	for i in range(num):
		print i,j
		if  ((poly[i][1] > y) != (poly[j][1] > y)) and \
	                (x < (poly[j][0] - poly[i][0]) * (y - poly[i][1]) / (poly[j][1] - poly[i][1]) + poly[i][0]):
			print poly[i][1], ">", y, poly[j][1] ,">" ,y
			print poly[j][0] - poly[i][0], y - poly[i][1] 
			c = not c
		j = i
	return c


t = isPointInPath(2,3, [(1,2),(4,2),(2,4)])
print t

ofxTriangle
-----------
- Given a set of points from a complex polygon, triangulates the points of the smallest number of triangles required to fill that polygon.

- Particularly useful for breaking down concave shapes (which can't be drawn natively by OpenGL) into triangles (which OpenGL is happy with)

- An example is here:
http://forum.openframeworks.cc/index.php/topic,10582.0.html

Credits
-------
- Uses the Triangle library, a 'Two-Dimensional Quality Mesh Generator and Delaunay Triangulator', by Jonathan Richard Shewchuk.

- Uses the 'point inside polygon' algorithm by Theo Watson.

Examples
--------
- example-opencv by James George - requires ofxOpenCv - builds polygons based on frame differencing from your webcam

- example-simple by Andrew McWilliams - requires no addons - just demonstrates a concave polygon
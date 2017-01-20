$fs = 0.25;
$fa = 8;

// size is the XY plane size, height in Z
module hexagon(size, height) {
  boxWidth = size/1.75;
  for (r = [-60, 0, 60]) rotate([0,0,r]) cube([boxWidth, size, height], true);
}

// screw bore extending downwards below z=0 plane 
// head extends a but above zero to allow for sinking it further
module screw2x6bore() {
    rotate([180, 0, 0]) {
        cylinder(d=2.2, h=6);
        translate ([0, 0, -1.3])
            cylinder(d1=2*3.6, d2=0, h=2*1.3);
    }
}

screw2x6bore();
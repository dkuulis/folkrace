$fs = 0.5;
$fa = 12;

module bar(p1, p2, d=1) {
    
    a = [p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]];
    l = norm(a);
    b = [a[0]/l, a[1]/l, a[2]/l];
    
    c = cross([0,0,1], b);
    
    az = atan2(c[0], c[1]);
    ax = acos(b[2]);
    
    //translate(p1) sphere(d=d, center=true);
    //translate(p2) sphere(d=d, center=true);
   
    translate(p1) 
        rotate([0, 0, -az])   
           rotate([0, ax, 0])
               //cylinder(d=d, h=l, center=false);
               translate([0, 0, l/2])
                    cube(size=[d, d, l+d], center=true);
        
}

module sweep(a1, a2, b1, b2, n=10, d=1) {
    for (i=[0:n]) {
        t1 = i/n;
        t2 = 1-t1;
        a = [t1*a1[0]+t2*a2[0], t1*a1[1]+t2*a2[1], t1*a1[2]+t2*a2[2]];
        b = [t1*b1[0]+t2*b2[0], t1*b1[1]+t2*b2[1], t1*b1[2]+t2*b2[2]];
       
        bar(a, b, d);
    }
}

module hyper(a, c, b, n=10, d=1) {
    sweep(a, c, c, b, n, d);
}

module hyper3d(p1, p2, p3, c, n=10, d=1) {
    sweep(p1, c, c, p2, n, d);
    sweep(p1, c, c, p3, n, d);
    sweep(p2, c, c, p3, n, d);
}

module sheet(a1, a2, b1, b2, n=10, d=1) {
    sweep(a1, a2, b1, b2, n, d);
    sweep(a1, a2, b1, b1, n, d);
    sweep(a2, a2, b1, b2, n, d);

    bar(a1, a2, d);
    bar(b1, b2, d);
}


module skew(dims) {
    matrix = [
        [ 1, dims[0], dims[1], 0 ],
        [ dims[2], 1, dims[4], 0 ],
        [ dims[5], dims[3], 1, 0 ],
        [ 0, 0, 0, 1 ]
    ];

    multmatrix(matrix) children();
}

module cagetop()
{
            for (x = [-20, 0, 20, 40]) translate([x-10, 0, 81]) cube(size=[2, 60, 2], center=true);
            for (y = [-30, 0, 30])    translate([0, y, 81])  cube(size=[60, 2, 2], center=true);

            translate([43, 0, 67]) skew([0, -0.9, 0, 0, 0, 0]) cube(size=[2, 2, 30], center=true);

            // front sides
            translate([33, -33, 67]) skew([0, -0.2, 0, 0, 0.2, 0]) cube(size=[2, 2, 30], center=true);
            translate([33,  33, 67]) skew([0, -0.2, 0, 0, -0.2, 0]) cube(size=[2, 2, 30], center=true);

            // rear
            translate([-42, -34.5, 67]) skew([0, 0.8, 0, 0, 0.3, 0]) cube(size=[2, 2, 30], center=true);
            translate([-42,  34.5, 67]) skew([0, 0.8, 0, 0, -0.3, 0]) cube(size=[2, 2, 30], center=true);
}

module cage(round=false)
{
    if (round) minkowski() {
        cagetop();
        sphere(r=1);
    } else
        cagetop();

    #translate([ 36,  36, 45]) cylinder(d=3, h=7, center=false);
    #translate([ 36, -36, 45]) cylinder(d=3, h=7, center=false);

    #translate([-54, -39, 45]) cylinder(d=3, h=7, center=false);
    #translate([-54,  39, 45]) cylinder(d=3, h=7, center=false);
}

//sweep([0,0,100], [0,0,0], [0,0,0], [100,0,0], 10);

//cage(false);


union() {
    
    d = 4;
    n = 4;
    
    top = 81;
    btm = 52;
    
    lft = 30;
    rgh = -30;
    
    front = 30;
    mid = 10;
    rear = -30;
    
    cntr = 0;

    //for (x = [front, mid, rear]) bar([x, lft, top], [x, rgh, top]);
    //for (y = [lft, cntr, rgh]) bar([front, y, top], [rear, y, top]);
    
    btm_fl = [36, 36, btm];
    btm_fr = [36, -36, btm];
    btm_rl = [-54, 39, btm];
    btm_rr = [-54, -39, btm];

    btm_fc = [60, 0, btm];

    top_fl = [front, lft, top];
    top_fr = [front, rgh, top];
    top_rl = [rear, lft, top];
    top_rr = [rear, rgh, top];
 
 
//    hyper3d(btm_fl, [mid, lft, top], [front, cntr, top], top_fl, n, d);
//    hyper3d(btm_fr, [mid, rgh, top], [front, cntr, top], top_fr, n, d);
//    hyper3d(btm_rl, [mid, lft, top], [rear, cntr, top], top_rl, n, d);
//    hyper3d(btm_rr, [mid, rgh, top], [rear, cntr, top], top_rr, n, d);
    
    //hyper(btm_fc, [front, cntr, top], top_fl, 10, d); 
    //hyper(btm_fc, [front, cntr, top], top_fr, 10, d); 
    
    sheet(btm_fc, [front, cntr, top], top_fl, btm_fl, n, d); 
    sheet(btm_fc, [front, cntr, top], top_fr, btm_fr, n, d); 

    sheet(btm_fl, top_fl, top_rl, btm_rl, n, d); 
    sheet(btm_fr, top_fr, top_rr, btm_rr, n, d); 

    sheet(btm_rr, top_rr, top_rl, btm_rl, n, d); 
    //sheet([mid, rgh, top], top_rr, top_rl, [mid, lft, top], n, d); 


    //sheet(top_rr, top_fr, top_fl, top_rl, n, d); 

}
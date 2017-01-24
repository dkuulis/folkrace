//HC-SR04 Ultrasonic distance sensor module
//Mark Benson
//23/07/2013
//Creative Commons non commercial

module hcsr04() {
    //difference() {
        union()
        {
            color("DARKBLUE") cube([45,20,1.2]);
            translate([16/2+1,18/2+1,1.3]) ultrasonicSensor();
            translate([16/2+27+1,18/2+1,1.3]) ultrasonicSensor();
            color("SILVER") translate([45/2-3.25,20-4,1.3]) xtal();

            translate([(45/2)-((2.45*4)/2)+0.6,2,-0.2]) rotate([180,0,0]) headerPin(4);
        }

        // removing screw cutouts to improve rendering
        /*
        union()
        {
            color("DARKBLUE") translate([2.5,2.5,-1]) cylinder(d=2, h=3);
            color("DARKBLUE") translate([2.5+40,2.5+15,-1]) cylinder(d=2, h=3);
        }

    } */
}

module ultrasonicSensor()
{
    union()
    {
        //Sensor body
        color("SILVER") difference()
        {
            //Body
            translate([0,0,0]) cylinder(d=16, h=13.8);
            //Hole in body
            translate([0,0,0]) cylinder(d=12.5, h=14);
        }

        //Insert to indicate mesh
        color("GREY") translate([0,0,0]) cylinder(d=12.5, h=13);
    }
}

module xtal()
{
    //Base
    hull()
    {
        translate([0,0,0]) cylinder(d=4.65, h=0.4);
        translate([6.5,0,0]) cylinder(d=4.65, h=0.4);
    }

    //Body
    hull()
    {
        translate([0,0,0]) cylinder(d=3.7, h=3.56);
        translate([6.5,0,0]) cylinder(d=3.7, h=3.56);
    }
}

module headerPin(numberOfPins)
{
    for (i = [0:numberOfPins-1])
    {
        color("GOLD") translate([(i*2.54),0,-2]) cube([0.5,0.5,8]);
        color("BLACK") translate([(i*2.54-2.54/2+0.25),-2.54/2+0.25,0]) cube([2.54,2.54,1]);
    }
}

hcsr04();
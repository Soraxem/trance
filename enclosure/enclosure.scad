
$fa = $preview ? 1 : 1;
$fs = $preview ? 0.1 : 0.1;

thread = 3;
thread_height = 6;
thickness = 2; // try to use multibles of 0.4 because of the nozzle
thickness_vector = [thickness, thickness, 1];

size = [120, 80, 40];
radius = 10;

// Roundedcube module inspired by Daniel Upshaw
module roundedcube(size = [1,1,1], radius = 0.5) {
    // Create 4 cylinders for all corners of the cube, then the hull of them is made
    hull() {
        translate([size[0]/2-radius, size[1]/2-radius, 0])cylinder(h = size[2], r = radius, center = true);
        translate([-size[0]/2+radius, size[1]/2-radius, 0])cylinder(h = size[2], r = radius, center = true);
        translate([size[0]/2-radius, -size[1]/2+radius, 0])cylinder(h = size[2], r = radius, center = true);
        translate([-size[0]/2+radius, -size[1]/2+radius, 0])cylinder(h = size[2], r = radius, center = true);
    }
}

// Thread module inspired by made with layers
module thread(length = 20, diameter = 3) {
    difference() {
        translate([0,0,-length/2])
        cylinder(h = length, d = diameter, center = true);
  
        for (i = [0 : 360/3 : 360] ){
            rotate([0,0,i])
            translate([diameter/1.8,0,-length/2 - 1])
            cylinder(h = length, d = diameter/3, center = true);
            
            rotate([0,0,i])
            translate([diameter/1.8,0,-0.5])
            cylinder(h = 1, d2 = 0, d1 = diameter/3, center = true);
        }
    }
}


module standoff() {
    difference() {
        translate([0,0,-thread_height])
        cylinder(h=thread_height, d = thread + thickness*2);
        thread(thread_height,thread);
    }
}



// Case
difference() {
    translate([0,0,size.z/2 - thickness/2])
    roundedcube(size - [0,0,thickness], radius);
    
    translate([0,0,size.z/2 + thickness])
    roundedcube([size.x - 2*thickness, size.y - 2*thickness, size.z], radius - thickness);
    
}

// PCB mounting
translate([50,30,thread_height+thickness])standoff();
translate([-50,30,thread_height+thickness])standoff();
translate([50,-30,thread_height+thickness])standoff();
translate([-50,-30,thread_height+thickness])standoff();

// Lid Mounting
difference() {
hull() {
    translate([50,0,0])cylinder(h = size.z - thickness, d = thread + 2*thickness);
    translate([size.x/2-thickness/2,0,size.z/2 - thickness/2])
    cube([thickness, thread + 2*thickness, size.z - thickness], true);
}

translate([50,0,size.z-thickness])thread(size.z-thickness*2, thread);
}

difference() {
hull() {
    translate([-50,0,0])cylinder(h = size.z - thickness, d = thread + 2*thickness);
    translate([-size.x/2+thickness/2,0,size.z/2 - thickness/2])
    cube([thickness, thread + 2*thickness, size.z - thickness], true);
}

translate([-50,0,size.z-thickness])thread(size.z-thickness*2, thread);

}


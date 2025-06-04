
$fa = $preview ? 1 : 1;
$fs = $preview ? 0.3 : 0.2;

thread = 3;
thread_height = 5;
thickness = 2; // try to use multibles of 0.4 because of the nozzle
thickness_vector = [thickness, thickness, 1];
tolerance = 0.2;

size = [120, 80, 40];
radius = 10;

pcb_mount = [100, 60];
pcb_thickness = 1.6;

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
// Inspired by Mark Harrison on Stackoverflow
module rotate_about_point(rotation, point) {
    translate(point)
        rotate(rotation)
            translate(-point)
                children();   
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
// Creates the holes for the Neutrik DMX
module neutrik() {
    translate([0,0,12.5])
    rotate([90,0,0])
    
    union() {
        cylinder(h = 10, d = 22, center = true);
        
        translate([-19.8/2, 19.8/2, 0])
        cylinder(h = 10, d = 3.2, center = true);
        
        translate([19.8/2, -19.8/2, 0])
        cylinder(h = 10, d = 3.2, center = true);
     }
}

// USB C hole
module usb_c() {
    translate([0,0,3.4/2])
    rotate([90,0,0])
    roundedcube([9.1,3.6,10], 1.1);
}

module standoff() {
    difference() {
        translate([0,0,-thread_height])
        cylinder(h=thread_height, d = thread + thickness*2);
        thread(thread_height,thread);
    }
}

module lid() {
    
    // Plate
    translate([0,0,-thickness/2])
    difference() {
        roundedcube([size.x, size.y, thickness], radius);
        // Screwholes
        translate([50,0,0])cylinder(h=thickness+1,d=thread,center=true);
        translate([-50,0,0])cylinder(h=thickness+1,d=thread,center=true);
    }
    
    // Rim
    translate([0,0,-thickness/2 - thickness])
    difference() {
        roundedcube([size.x - 2*thickness - 2*tolerance, size.y  - 2*thickness - 2*tolerance, thickness], radius - thickness - tolerance);
        roundedcube([size.x - 4*thickness - 2*tolerance, size.y  - 4*thickness - 2*tolerance, thickness+1], radius - 2*thickness - tolerance);
        
        // Screw Standoff
        cube([size.x, thread + 2*thickness + 2*tolerance, thickness+1], true);
    }
    
}

module base() {

// Case
difference() {
    translate([0,0,size.z/2 - thickness/2])
    roundedcube(size - [0,0,thickness], radius);
    
    translate([0,0,size.z/2 + thickness])
    roundedcube([size.x - 2*thickness, size.y - 2*thickness, size.z], radius - thickness);
    
    translate([5,-40,thread_height + pcb_thickness + thickness])
    neutrik();
    
    translate([33,-40,thread_height + pcb_thickness + thickness])
    neutrik();
    
    translate([-40,-40,thread_height + pcb_thickness + thickness])
    usb_c();
    
}

// PCB mounting
translate([pcb_mount.x/2, pcb_mount.y/2, thread_height+thickness])standoff();
translate([-pcb_mount.x/2, pcb_mount.y/2, thread_height+thickness])standoff();
translate([pcb_mount.x/2, -pcb_mount.y/2, thread_height+thickness])standoff();
translate([-pcb_mount.x/2, -pcb_mount.y/2, thread_height+thickness])standoff();

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



}

translate([0,100,0])
rotate([180,0,0])
lid();

base();

translate([0,0,-6 * $t])
rotate_about_point([30 * $t,0,0],[0,-42,32])
color("lime")translate([0, 0, thread_height + thickness])import("../pcb/wifi-dmx/wifi-dmx.stl");



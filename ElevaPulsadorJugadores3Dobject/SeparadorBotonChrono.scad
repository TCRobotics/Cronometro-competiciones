borde= 1.4;
rpequeno= 30;
rgrande= 32.5;
h1= 10;
rgujero= 12.5;

difference() {
cylinder (r1=rgrande, r2=rpequeno, h=h1, $fn=128);
    cylinder (r=rgujero, h=20, $fn=64);
    translate ([ 37/2,0,0])   cylinder(r=2, h=20, $fn=24);
    translate ([-37/2,0,0])   cylinder(r=2, h=20, $fn=24 );
}

// Diseño por MiloBarral

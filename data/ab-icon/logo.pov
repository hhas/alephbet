#version 3.7;
#declare LITTLE_BALL_OFFSET = 1/3;
#declare LITTLE_BALL_SIZE = 1;
#declare CRACK_WIDTH = 1/12;
#ifndef(MARGIN)
#declare MARGIN = 2.04;
#end

global_settings {
  assumed_gamma 1.0
  ambient_light rgb<0.6,0.8,1>
}

camera {
  orthographic
  location <0,0,10>
  look_at <0,0,0>
  right -x * MARGIN
  up y * MARGIN
}

union {
  sphere {
    <0, LITTLE_BALL_OFFSET, 0>, LITTLE_BALL_SIZE * 0.5
  }
  difference {
    sphere {
      <0, 0, 0>, 1
    }
    cylinder {
      <0, LITTLE_BALL_OFFSET, -10>, <0, LITTLE_BALL_OFFSET, 10>, LITTLE_BALL_SIZE/2 + CRACK_WIDTH
    }
    box {
      <CRACK_WIDTH * -0.5, LITTLE_BALL_OFFSET, -10>, <CRACK_WIDTH * 0.5, -10, 10>
    }
  }
  texture {
    pigment {
      color rgb<1,0,0.25>
    }
  }
  finish {
    conserve_energy
    diffuse 1
    ambient 0.1
    specular albedo 0.1
    roughness 0.07
    irid { 0.1 thickness 0.75 }
  }
  no_shadow
}

light_source {
  <-5, 10, 10>
  color rgb<0.6,0.6,0.6>
  parallel
  point_at <0,0,0>
}

light_source {
  <5, -10, 10>
  color rgb<0.35,0.4,0.4>
  area_light x*3, y*3, 25, 25 
  shadowless
}


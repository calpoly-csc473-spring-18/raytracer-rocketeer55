// CSC 473 Final Project Scene 3

camera {
  location  <0, 0, 0>
  up        <0,  1,  0>
  right     <1.77778, 0,  0>
  look_at   <0, 0, -400>
}

// FIRST LIGHT
light_source {<0, 25, 300> color rgb <1, 0, 0>}

// SECOND LIGHT
light_source {<0, 25, 200> color rgb <0, 0, 1>}

// THIRD LIGHT
light_source {<0, 25, 100> color rgb <1, 0, 0>}


// FIRST LIGHT SPHERE
sphere {<0, 25, 300>, 2
  pigment {color rgb <1, 0.0, 0.0>}
  finish {ambient 1}
  is_light
}

// SECOND LIGHT SPHERE
sphere {<0, 25, 200>, 2
  pigment {color rgb <0.0, 0.0, 1>}
  finish {ambient 1}
  is_light
}

// THIRD LIGHT SPHERE
sphere {<0, 25, 100>, 2
  pigment {color rgb <1, 0.0, 0.0>}
  finish {ambient 1}
  is_light
}


// FLOOR
plane {<0, 1, 0>, -10
  pigment {color rgb <0.93333, 0.91375, 0.91375>}
  finish {ambient 0.1 diffuse 0.6}
}

// LEFT WALL
plane {<-1, 0, 0>, -15
  pigment {color rgb <0.93333, 0.91375, 0.91375>>}
  finish {ambient 0.1 diffuse 0.6}
}

// RIGHT WALL
plane {<-1, 0, 0>, 15
  pigment {color rgb <0.93333, 0.91375, 0.91375>>}
  finish {ambient 0.1 diffuse 0.6}
}
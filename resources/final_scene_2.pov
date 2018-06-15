// CSC 473 Final Project Scene 2

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.77778, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.0, 0.94510, 0.87843>}

// SPHERE
sphere { <5, 4, 0>, 2
  pigment { color rgb <0.69804, 0.07059, 0.02745>}
  finish {ambient 0.2 diffuse 0.4}
  translate <0, 0, 0>
}

// FLOOR
plane {<0, 1, 0>, -4
    pigment {color rgb <0.93333, 0.91375, 0.91375>>}
    finish {ambient 0.4 diffuse 0.8}
}

// BACK
plane {<0, 0, 1>, -20
    pigment {color rgb <0.93333, 0.91375, 0.91375>>}
    finish {ambient 0.4 diffuse 0.8}
}

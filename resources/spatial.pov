// Project4 custom .pov file - Spencer Schurk

camera {
  location  <0, 0, 12>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 80> color rgb <1.5, 1.5, 1.5>}

sphere { <0, 0, 0>, 3
  pigment { color rgb <1.0, 1.0, 0>}
  finish {ambient 0.5 diffuse 0.4 specular 0.5 roughness 0.05}
}

sphere { <1, 0.2, 2.7>, 0.8
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish {ambient 0.8 diffuse 0.4 specular 0.6 roughness 0.05, ior 1.3}
}

sphere { <-1, 0.2, 2.7>, 0.8
  pigment { color rgbf <1.0, 1.0, 1.0, 0.5>}
  finish {ambient 0.8 diffuse 0.4 specular 0.6 roughness 0.05, ior 1.3}
}

sphere { <-1.8, 0.2, 6.7>, 0.5
  pigment { color rgb <0, 0, 0>}
  finish {ambient 0.1 diffuse 0.4 specular 0.6 roughness 0.05}
  scale <0.5, 1, 0.5>
}

sphere { <1.8, 0.2, 6.7>, 0.5
  pigment { color rgb <0, 0, 0>}
  finish {ambient 0.1 diffuse 0.4 specular 0.6 roughness 0.05}
  scale <0.5, 1, 0.5>
}

triangle {
  <-1.7, -0.3, 3>,
  <0, -0.8, 4 >,
  <1.7, -0.3, 3>
  pigment {color rgb <0, 0, 0>}
  finish {ambient 1.0, diffuse 0.3}
}

box {<-3, 2.2, -2.5>, <3, 3.2, 2.5>
  pigment { color rgb <0.2, 0.2, 0.2>}
  finish {ambient 0.2 diffuse 0.8 specular 0.6 roughness 0.2 reflection 0.1}
}

box {<-1.5, 3.2, -1.5>, <1.5, 5, 1.5>
  pigment { color rgb <0.2, 0.2, 0.2>}
  finish {ambient 0.2 diffuse 0.8 reflection 0.1}
}

plane {<0, 1, 0>, -4
  pigment {color rgb <0.6, 0.2, 1.0>}
  finish {ambient 0.3 diffuse 0.8 specular 0.8 roughness 0.001 reflection 0.5}
}

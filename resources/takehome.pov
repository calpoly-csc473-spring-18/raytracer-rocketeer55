// csc473 :: Assignment 6 custom file

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<0, 20, -20> color rgb <1.5, 1.5, 1.5>}

box {<-10, -4, -100>, <-5, 50, 0>
	pigment {color rgb <1.0, 0.0, 0.0>}
	finish {ambient 0.5 diffuse 0.8}
}

box {<5, -4, -100>, <10, 50, 0>
    pigment {color rgb <0.0, 0.0, 1.0>}
    finish {ambient 0.5 diffuse 0.8}
}

plane {<0, 1, 0>, -4
      pigment {color rgb <0.0, 0.0, 0.0>}
      finish {ambient 0.5 diffuse 0.8}
}

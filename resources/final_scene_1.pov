// CSC 473 Final Project Scene 1

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.77778, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<0, 0, 0> color rgb <1.0, 0.94510, 0.87843>}

// Floor (y = -8)
plane {<0, 1, 0>, -5
	pigment {color rgb <0.93333, 0.91375, 0.91375>}
    finish {ambient 0.2 diffuse 0.3}
}

// Left wall (x = -8)
plane {<-1, 0, 0>, -8
	pigment {color rgb <0.93333, 0.91375, 0.91375>}
    finish {ambient 0.2 diffuse 0.3}
}

// Right wall (x = 8)
plane {<-1, 0, 0>, 8
	pigment {color rgb <0.93333, 0.91375, 0.91375>}
    finish {ambient 0.2 diffuse 0.3}
}

// Top wall (y = 8)
plane {<0, 1, 0>, 9
	pigment {color rgb <0.93333, 0.91375, 0.91375>}
    finish {ambient 0.2 diffuse 0.3}
}

// Back wall (z = 8)
plane {<0, 0, 1>, -8}
	pigment {color rgb <0.93333, 0.91375, 0.91375>}
    finish {ambient 0.2 diffuse 0.3}
}

// SPHERE
sphere {<0, 0, 0>, 1
	pigment {color rgb <0.93333, 0.91375, 0.91375>}
    finish {ambient 0.1 diffuse 0.6 specular 0.1}
}

// LIGHT SPHERE
sphere {<0, 0, 0>, 0.25
    pigment {color rgb <1, 1, 1>}
    finish {ambient 1}
    is_light
}

// CUBE FRAME
// Vertical - back left
box {<-4.1, -4.1, -4.1>, <-3.9, 4.1, -3.9>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Vertical - back right
box {<3.9, -4.1, -4.1>, <4.1, 4.1, -3.9>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Vertical - front left
box {<-4.1, -4.1, 3.9>, <-3.9, 4.1, 4.1>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Vertical - front right
box {<3.9, -4.1, 3.9>, <4.1, 4.1, 4.1>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Horizontal - back bottom
box {<-4.1, -4.1, -4.1>, <4.1, -3.9, -3.9>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Horizontal - back top
box {<-4.1, 3.9, -4.1>, <4.1, 4.1, -3.9>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Horizontal - front bottom
box {<-4.1, -4.1, 3.9>, <4.1, -3.9, 4.1>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Horizontal - front top
box {<-4.1, 3.9, 3.9>, <4.1, 4.1, 4.1>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Sideways - left bottom
box {<-4.1, -4.1, -4.1>, <-3.9, -3.9, 4.1>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Sideways - right bottom
box {<3.9, -4.1, -4.1>, <4.1, -3.9, 4.1>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Sideways - left top
box {<-4.1, 3.9, -4.1>, <-3.9, 4.1, 4.1>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}

// Sideways - right top
box {<3.9, 3.9, -4.1>, <4.1, 4.1, 4.1>
	pigment {color rgb <0.27843, 0.27451, 0.27843>}
    finish {ambient 0.4 diffuse 0.2}
}
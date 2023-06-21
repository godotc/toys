

def volume_of_hole_DJ ( l:int , w:int , h:int , k:int , work_face :int):
    a = l+ work_face*2 + k*h
    b = w+ work_face*2 + k*h
    return a*b * h + (k**2* h**3) /3









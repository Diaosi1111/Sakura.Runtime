require test_annotation

[export]
def test
    print("this tutorial binds custom type with fields and properties," 
        + " as well as function which takes and returns this type\n")
    let c = [[Color g=uint8(0x40), a=uint8(0xff), f2=float2(1.0, 2.0)]]
    print("c = {c}, luminance is {c.luminance}\n")
    // let cg = make_gray(c)
    // print("cg = {cg}\n")
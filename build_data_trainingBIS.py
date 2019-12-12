

def build_training():

    lsource = []
    lnew = []
    
    source10 = open("data10.txt","r")
    lsource.append(source10)
    
    source11 = open("data11.txt","r")
    lsource.append(source11)
                   
    source12 = open("data12.txt","r")
    lsource.append(source12)
                   
    source13 = open("data13.txt","r")
    lsource.append(source13)
                   
    source14 = open("data14.txt","r")
    lsource.append(source14)
                   
    source15 = open("data15.txt","r")
    lsource.append(source15)
                   
    source16 = open("data16.txt","r")
    lsource.append(source16)
                   
    source17 = open("data17.txt","r")
    lsource.append(source17)
                   
    source18 = open("data18.txt","r")
    lsource.append(source18)
                   
    source19 = open("data19.txt","r")
    lsource.append(source19)
                   
    source20 = open("data20.txt","r")
    lsource.append(source20)
                   
    source21 = open("data21.txt","r")
    lsource.append(source21)
                   
    source22 = open("data22.txt","r")
    lsource.append(source22)
                   
    source23 = open("data23.txt","r")
    lsource.append(source23)
                   
    source24 = open("data24.txt","r")
    lsource.append(source24)
                   
    source25 = open("data25.txt","r")
    lsource.append(source25)
                   
    source26 = open("data26.txt","r")
    lsource.append(source26)
                   
    source27 = open("data27.txt","r")
    lsource.append(source27)
                   
    source28 = open("data28.txt","r")
    lsource.append(source28)
                   
    source29 = open("data29.txt","r")
    lsource.append(source29)
                   
    source30 = open("data30.txt","r")
    lsource.append(source30)
                   
    source31 = open("data31.txt","r")
    lsource.append(source31)
                   
    source32 = open("data32.txt","r")
    lsource.append(source32)
                   
    source33 = open("data33.txt","r")
    lsource.append(source33)
                   
    source34 = open("data34.txt","r")
    lsource.append(source34)
                   
    source35 = open("data35.txt","r")
    lsource.append(source35)








    new10 = open("training10.txt","a")
    lnew.append(new10)
    
    new11 = open("training11.txt","a")
    lnew.append(new11)
                   
    new12 = open("training12.txt","a")
    lnew.append(new12)
                   
    new13 = open("training13.txt","a")
    lnew.append(new13)
                   
    new14 = open("training14.txt","a")
    lnew.append(new14)
                   
    new15 = open("training15.txt","a")
    lnew.append(new15)
                   
    new16 = open("training16.txt","a")
    lnew.append(new16)
                   
    new17 = open("training17.txt","a")
    lnew.append(new17)
                   
    new18 = open("training18.txt","a")
    lnew.append(new18)
                   
    new19 = open("training19.txt","a")
    lnew.append(new19)
                   
    new20 = open("training20.txt","a")
    lnew.append(new20)
                   
    new21 = open("training21.txt","a")
    lnew.append(new21)
                   
    new22 = open("training22.txt","a")
    lnew.append(new22)
                   
    new23 = open("training23.txt","a")
    lnew.append(new23)
                   
    new24 = open("training24.txt","a")
    lnew.append(new24)
                   
    new25 = open("training25.txt","a")
    lnew.append(new25)
                   
    new26 = open("training26.txt","a")
    lnew.append(new26)
                   
    new27 = open("training27.txt","a")
    lnew.append(new27)
                   
    new28 = open("training28.txt","a")
    lnew.append(new28)
                   
    new29 = open("training29.txt","a")
    lnew.append(new29)
                   
    new30 = open("training30.txt","a")
    lnew.append(new30)
                   
    new31 = open("training31.txt","a")
    lnew.append(new31)
                   
    new32 = open("training32.txt","a")
    lnew.append(new32)
                   
    new33 = open("training33.txt","a")
    lnew.append(new33)
                   
    new34 = open("training34.txt","a")
    lnew.append(new34)
                   
    new35 = open("training35.txt","a")
    lnew.append(new35)



    for j in range(len(lsource)):
        if j!=0:
            for ligne in lsource[j]:
                new10.write(ligne)
            for ligne in source10:
                new10.write(ligne)
            source10 = open("data10.txt","r")
    
    
build_training()

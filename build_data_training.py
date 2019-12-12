

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








    new10 = open("data10.txt","a")
    lnew.append(new10)
    
    new11 = open("data11.txt","a")
    lnew.append(new11)
                   
    new12 = open("data12.txt","a")
    lnew.append(new12)
                   
    new13 = open("data13.txt","a")
    lnew.append(new13)
                   
    new14 = open("data14.txt","a")
    lnew.append(new14)
                   
    new15 = open("data15.txt","a")
    lnew.append(new15)
                   
    new16 = open("data16.txt","a")
    lnew.append(new16)
                   
    new17 = open("data17.txt","a")
    lnew.append(new17)
                   
    new18 = open("data18.txt","a")
    lnew.append(new18)
                   
    new19 = open("data19.txt","a")
    lnew.append(new19)
                   
    new20 = open("data20.txt","a")
    lnew.append(new20)
                   
    new21 = open("data21.txt","a")
    lnew.append(new21)
                   
    new22 = open("data22.txt","a")
    lnew.append(new22)
                   
    new23 = open("data23.txt","a")
    lnew.append(new23)
                   
    new24 = open("data24.txt","a")
    lnew.append(new24)
                   
    new25 = open("data25.txt","a")
    lnew.append(new25)
                   
    new26 = open("data26.txt","a")
    lnew.append(new26)
                   
    new27 = open("data27.txt","a")
    lnew.append(new27)
                   
    new28 = open("data28.txt","a")
    lnew.append(new28)
                   
    new29 = open("data29.txt","a")
    lnew.append(new29)
                   
    new30 = open("data30.txt","a")
    lnew.append(new30)
                   
    new31 = open("data31.txt","a")
    lnew.append(new31)
                   
    new32 = open("data32.txt","a")
    lnew.append(new32)
                   
    new33 = open("data33.txt","a")
    lnew.append(new33)
                   
    new34 = open("data34.txt","a")
    lnew.append(new34)
                   
    new35 = open("data35.txt","a")
    lnew.append(new35)



    for i in range(len(lnew)):
        for j in range(len(lsource)):
            if j!=i:
                for ligne in lsource[j]:
                    lnew[i].write(ligne)
                for ligne in lsource[i]:
                    lnew[i].write(ligne)
    
    
build_training()

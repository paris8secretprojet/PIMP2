
def trier(nom):

    source = open(nom,"r")

    for ligne in source:
        l = ligne.split(",")

        if(l[0]=="10"):
            fichier = open("data10.txt","a")
            fichier.write(ligne)

        elif(l[0]=="11"):
            fichier = open("data11.txt","a")
            fichier.write(ligne)

        elif(l[0]=="12"):
            fichier = open("data12.txt","a")
            fichier.write(ligne)

        elif(l[0]=="13"):
            fichier = open("data13.txt","a")
            fichier.write(ligne)

        elif(l[0]=="14"):
            fichier = open("data14.txt","a")
            fichier.write(ligne)

        elif(l[0]=="15"):
            fichier = open("data15.txt","a")
            fichier.write(ligne)

        elif(l[0]=="16"):
            fichier = open("data16.txt","a")
            fichier.write(ligne)

        elif(l[0]=="17"):
            fichier = open("data17.txt","a")
            fichier.write(ligne)

        elif(l[0]=="18"):
            fichier = open("data18.txt","a")
            fichier.write(ligne)

        elif(l[0]=="19"):
            fichier = open("data19.txt","a")
            fichier.write(ligne)

        elif(l[0]=="20"):
            fichier = open("data20.txt","a")
            fichier.write(ligne)

        elif(l[0]=="21"):
            fichier = open("data21.txt","a")
            fichier.write(ligne)

        elif(l[0]=="22"):
            fichier = open("data22.txt","a")
            fichier.write(ligne)

        elif(l[0]=="23"):
            fichier = open("data23.txt","a")
            fichier.write(ligne)

        elif(l[0]=="24"):
            fichier = open("data24.txt","a")
            fichier.write(ligne)

        elif(l[0]=="25"):
            fichier = open("data25.txt","a")
            fichier.write(ligne)

        elif(l[0]=="26"):
            fichier = open("data26.txt","a")
            fichier.write(ligne)

        elif(l[0]=="27"):
            fichier = open("data27.txt","a")
            fichier.write(ligne)

        elif(l[0]=="28"):
            fichier = open("data28.txt","a")
            fichier.write(ligne)

        elif(l[0]=="29"):
            fichier = open("data29.txt","a")
            fichier.write(ligne)

        elif(l[0]=="30"):
            fichier = open("data30.txt","a")
            fichier.write(ligne)

        elif(l[0]=="31"):
            fichier = open("data31.txt","a")
            fichier.write(ligne)

        elif(l[0]=="32"):
            fichier = open("data32.txt","a")
            fichier.write(ligne)

        elif(l[0]=="33"):
            fichier = open("data33.txt","a")
            fichier.write(ligne)

        elif(l[0]=="34"):
            fichier = open("data34.txt","a")
            fichier.write(ligne)

        elif(l[0]=="35"):
            fichier = open("data35.txt","a")
            fichier.write(ligne)

        elif(l[0]=="36"):
            fichier = open("data36.txt","a")
            fichier.write(ligne)

def main():

    trier("emnist-balanced-train.txt")


main()

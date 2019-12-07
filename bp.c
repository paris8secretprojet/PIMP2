Prenons cette structure:

i1            (weights[i1,i2])     h1 (outputs[1,2])     o1  (weights[1,2])



i2            (weights[i3,i4])     h2                    o2  (weights[3,4])


  i1 = 0.05
  i2 = 0.10
  
  wi1 (i1-h1) = 0.15
  wi2 (i2-h1) = 0.25
  wi3 (i2-h1) = 0.25
  wi4 (i2-h2) = 0.30

  wi1 (h1-o1) = 0.40
  wi2 (h2-o1) = 0.45
  wi3 (h1-o1) = 0.50
  wi4 (h2-o2) = 0.55

  o1 expected = 0.01
  o2 expected = 0.99

// pour la derniere couche, on prend la somme totale de toutes les erreurs

// on prend l'output de chaque neurone et on le compare a ce qu'on attend

// alors si on fait l'apprentissage pour A, si '10' est le premier terme du vecteur de donnees, '0.999' est attendu a la fin, et si c'est autre chose que '10', '0.001' sera attendu.

// donc pour chaque erreur on prend (1/2)*((attendu - resultat)^2).

// on prend la somme de toutes les erreurs sur nos (par exemple) 62400 images, donc la somme des moities des carres.

// la derive partielle pour chaque neurone devient simplement (attendu-resultat) et on met ca au negatif, donc -(attendu-resultat).

// l'erreur pour le poid correspondant au  premier neurone de la couche suivant (en allant vers la gauche) est alors:
//    les poids de chaque neurone de la couche d'avant fois leurs valeurs.
//    On prend alors resultat(1-resultat) si on utilise la derive du sigmoid.



0.74 * 0.18 * 0.59 = 0.08

  


  netinput of o1 changing with respect to weight from h1 to o1

 deriv_Error/deriv_w1 =  -(target o1 - output o1) * output o1 * (1 - output1) * output h1

 deriv_Error/deriv_w2 =  -(target o1 - output o1) * output o1 * (1 - output1) * output h2

 deriv_Error/deriv_w3 =  -(target o2 - output o2) * output o2 * (1 - output2) * output h1

 deriv_Error/deriv_w4 =  -(target o2 - output o2) * output o2 * (1 - output2) * output h2
  
  (output h1 = sigmoid(h1 inputs))

   w1_new = w1 - learning_rate * (deriv_Error/deriv_w1)
   w2_new = w2 - learning_rate * (deriv_Error/deriv_w2)

   w3_new = w3 - learning_rate * (deriv_Error/deriv_w3)
   w4_new = w4 - learning_rate * (deriv_Error/deriv_w4)



  deriv_out h1 / deriv net_h1 = output h1
  
   
  wi1_new = wi1 - step_size * deriv_H1 * i1





  .000438 = 0.03635 * 0.2413 + 0.05


0.2413  = (output h1 * (1 - output h1))

  0.03635 = 0.055399 + -0.019049 = (0.138498 * 0.40) + -0.019049
  
deriv_Error o1 / deriv output h1 = ((0.74136 * 0.1868) * 0.40)

deriv_Error o1 / deriv output h1 =  -(target o1 - output o1) * ((output o1 * (1 - output o1))) * w1)

deriv_Error o2 / deriv output h1 =  -(target o2 - output o2) * ((output o2 * (1 - output o2))) * w2
  


  dError_total / dwi1 = ((deriv_Error o1 / deriv output h1) + (deriv_Error o2 / deriv output h1)) * (output h1 * (1 - output h1)) * i1


  wi1_new = wi1 - step-size * dError_total / dwi1

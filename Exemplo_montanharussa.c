process Carrinho [i=1 to n] {

  while(true) {

      for (i = 0; i < C; i++) V(embarque);
      P(todosABordo);

      //liga o clock
      tempo[i] += clock();

      passeio();

      //terminou passeio ou precisou parar, desliga o clock
      P(parado[i]);
      desliga clock() para carrinho i;

      //terminou o passeio
      if (tempo[i] % tempo_de_passeio == 0)
          for (i = 0; i < C; i++) V(desembarque);

      //espera carrinho da frente voltar a andar de novo 
     else {

         P(movendo[i+1]);

         //retoma o passeio de onde parou;

      }


  }//end while
}

 

process Trilho {

    while(true) {
    //checa os tempos entre dois carrinhos sucessivos

    //sinaliza se carrinho i-1 será parado
    for (i = n; i > 1; i--)
    //Pode ser parado se carrinho da frente parou ou se o tempo do passeio terminou (está na plataforma)
        if (tempo[i] - tempo[(i-1) % n] < eps || tempo[(i-1) % n] % tempo_de_passeio == 0)
            V(parado[(i-1)%1]);

    //sinaliza se carrinho da frente voltou a andar
    for (i = n; i > 1; i--)
        if (tempo[i] - tempo[(i-1)%n] >= eps)
            V(movendo[i]);

  }
}


Código da aula passada:

process Passageiro [i=1 to m] {

  while(true) {
    P(embarque);
    P(mutex);
    total++;
    if (total == C) V(todosABordo);
    V(mutex);
    passeio();
    P(desembarque);
    P(mutex); 
    total --;
    if(total == 0) V(todosFora);
    V(mutex);
   }
}

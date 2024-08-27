NUMBER fator1 = 1;
NUMBER fator2 = 2;

WHILE(fator1 < 11) {
    fator2 = 1;
    WHILE(fator2 < 11) {
      >> fator1 >> " x " >> fator2 >> " = " >> fator1 * fator2;
      fator2 = fator2 + 1;
    }
    >> "*******************";
    >> " ";
    fator1 = fator1 + 1;
}

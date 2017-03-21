#  EP2 - Programação Concorrente

## Introdução
Este exercı́cio-programa tem o objetivo de exercitar o uso de threads e primitivas de sincronização para implementar um programa concorrente, conforme requisitos descritos no enunciado abaixo.

### A Festa Secreta
Anualmente acontece na USP uma festa cujos convites são limitados a n alunos. Esse evento acontece em uma sala inconspı́cua, previamente definida, em um dos institutos. Quando os convidados chegam à festa, eles mostram o convite, entram e se divertem pela madrugada afora. A sala pode comportar qualquer número de alunos, porém restrito a n convidados. Ao longo da noite, alunos deixam a sala, cansados da festa e vão embora. Existe um único segurança no instituto que faz rondas durante a noite e que pode interromper a festa. O segurança entra na sala quando não houver nenhum aluno na festa, ou se o número de alunos dentro da sala for igual ou maior que p, p  n alunos. Se não houver alunos, ele inspeciona a sala e volta para a sua ronda. Mas se houver ao menos p alunos presentes, ele fica bravo e espera todos os alunos da sala irem embora, para depois voltar à ronda. Enquanto o segurança estiver na sala, nenhum aluno novo poderá entrar, mas alunos dentro da sala poderão sair. A festa só termina quando todos os n convidados participarem.

## Execução

Para rodar o EP basta descompactar o arquivo e dar um make pelo terminal
```sh
make
```
o que irá gerar o arquivo ep2, depois é só rodá-lo usando ./ep2 e os argumentos.

### Modo de uso
```sh
./ep2 <convidados> <minimo-alunos> <intervalo> <tempo-maximo> <tempo-ronda>
```

Onde:
<convidados>: 			numero total de convidados da festa
<minimo-alunos>: 		numero minimo de alunos na festa para o seguranca esvaziar a sala
<intervalo>: 			intervalo maximo de tempo dentre chegadas de convidados em ms
<tempo-maximo>: 		tempo maximo de participacao na festa para cada aluno em ms
<tempo-ronda>: 			tempo maximo de ronda do seguranca em ms


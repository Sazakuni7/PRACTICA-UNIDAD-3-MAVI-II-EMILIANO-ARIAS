Ejercicio 1:

-Cada una de las paredes simula elasticidad con una restitución de 1.0f o mayor.
-Se puede modificar la gravedad a 0.f para quitarla por completo, o 9.8 para la normal.
-Con click se puede crear un objeto flotante (triángulo dinámico) con mas densidad (por lo tanto mayor resistencia al rebote) y puede colisionar con la pelota.
-Con las flechas del teclado aplicamos fuerza direccionan a la pelota (Comentado/Opcional)

Ejercicio 2:


Ejercicio 4:

-Se utiliza el método AplicarFuerza que tiene la clase Avatar
-Movemos la caja con las flechas izquierda y derecha.
-Tiene un 0.1f de restitución para simular un pequeño rebote.
-Tiene 0.4f de fricción al igual que el suelo

Ejercicio 5:

-Podemos clickear cualquier zona de cualquier miembro del ragdoll y arrastrar.
-Usamos MouseJoint
-Intenté usar como en la teoría dice FrequencyHz y DampingRatio, pero me dice que no son miembros, así que los reemplacé por Stiffness y Damping respectivamente (Rigidez y amortiguación)

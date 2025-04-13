Ejercicio 1:

-Cada una de las paredes simula elasticidad con una restitución de 1.0f o mayor.
-Se puede modificar la gravedad a 0.f para quitarla por completo, o 9.8 para la normal.
-Con click se puede crear un objeto flotante (triángulo dinámico) con mas densidad (por lo tanto mayor resistencia al rebote) y puede colisionar con la pelota.
-Con las flechas del teclado aplicamos fuerza direccionan a la pelota (Comentado/Opcional)

Ejercicio 2:

-Se puede hacer click and drag en cualquier pelota con MouseJoint, se simula resorte entre el punto clickeado y el de arrastre.
-Intenté unir ambas pelotas con DistanceJoint pero no tuve éxito, incluso con stiffness y damping no me permitió ajustar la fuerza elástica. Además el MouseJoint competía con el DistanceJoint, generando movimientos bruscos. Investigué un poco y me decanté por probar a crear una clase Spring personalizada, y aquí tiene un control matemático exacto con la ley de Hooke (F = -k * Δx) donde:
k (stiffness) controla la rigidez.
Δx es la deformación desde la longitud de reposo.
-Al final este método no interfiere con MouseJoint porque lo aplico en UpdatePhysics(), un método de Game.

Ejercicio 3:

-Pelota unida a caja flotante por un Spring personalizado mencionado en el Ejercicio 2 (DistanceJoint también lo permite pero no hace resorte, solo péndulo)
-Hay MouseJoint para poder click and drag a la pelota y experimentar con el resorte

Ejercicio 4:

-Se utiliza el método AplicarFuerza que tiene la clase Avatar
-Movemos la caja con las flechas izquierda y derecha.
-Tiene un 0.1f de restitución para simular un pequeño rebote.
-Tiene 0.4f de fricción al igual que el suelo

Ejercicio 5:

-Podemos clickear cualquier zona de cualquier miembro del ragdoll y arrastrar.
-Usamos MouseJoint
-Intenté usar como en la teoría dice FrequencyHz y DampingRatio, pero me dice que no son miembros, así que los reemplacé por Stiffness y Damping respectivamente (Rigidez y amortiguación)

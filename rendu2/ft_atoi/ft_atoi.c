/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42school.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:44:49 by mvigara-          #+#    #+#             */
/*   Updated: 2024/11/13 16:44:59 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** KEY POINTS PARA ESTUDIAR:
** 1. isspace equivalentes:
**    - ' '  (32 en ASCII) espacio
**    - '\t' (9)  tab horizontal
**    - '\n' (10) nueva línea
**    - '\v' (11) tab vertical
**    - '\f' (12) form feed
**    - '\r' (13) retorno de carro
**
** 2. Conversión ASCII a número:
**    - Un carácter numérico en ASCII está entre '0' (48) y '9' (57)
**    - Para convertir a valor numérico: '5' - '0' = 5
**    
** 3. Manejo de signos:
**    - Un signo negativo invierte el resultado final
**    - Solo el primer signo cuenta
**    - Los signos deben estar justo antes del número
**
** 4. Acumulación del resultado:
**    - result = (result * 10) + (str[i] - '0')
**    - Multiplicamos por 10 para hacer espacio para el siguiente dígito
*/

int	ft_atoi(const char *str)
{
    int	result;
    int	sign;
    int	i;

    // Inicializamos variables
    result = 0;
    sign = 1;
    i = 0;

    // Saltamos espacios en blanco iniciales
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;

    // Manejamos el signo
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }

    // Convertimos dígitos a número
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = (result * 10) + (str[i] - '0');
        i++;
    }

    return (result * sign);
}
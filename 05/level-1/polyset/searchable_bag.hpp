/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   searchable_bag.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42school.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:23:22 by fatkeski          #+#    #+#             */
/*   Updated: 2026/08/10 21:49:39 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "bag.hpp"

class searchable_bag : virtual public bag // virtual inheritance
{
 public:
	virtual bool has(int) const = 0; // verilen sayı bag'te var mı yok mu kontrolünü yapar
};

//must support addition, substraction and multiplication!
#include "vect2.hpp"

vect2::vect2()
{
    this->_container.push_back(0);
    this->_container.push_back(0);
}

vect2::vect2(int v1, int v2)
{
    this->_container.push_back(v1);
    this->_container.push_back(v2);
}

vect2::vect2(const vect2& original)
{
    *this = original;
}

vect2::~vect2() {}

int vect2::getContainer_elements(int array) const
{
    return (this->_container[array]);
}


// Addition OPERATORS
    vect2	vect2::operator+(int add) const
    {
        vect2 copy(*this);
    
        copy._container[0] += add;
        copy._container[1] += add;
        return (copy);
    }

    vect2	vect2::operator+(const vect2 addon) const
    {
        vect2 copy(*this);

        copy._container[0] += addon._container[0];
        copy._container[1] += addon._container[1];
        return (copy);
    }


    vect2&	vect2::operator++(void)
    {
        this->_container[0] += 1;
        this->_container[1] += 1;
        return (*this);
    } 

    vect2	vect2::operator++(int)
    {
        vect2 copy(*this);

        this->_container[0] += 1;
        this->_container[1] += 1;
        return (copy);
    }

// substraction OPERATORS
    vect2	vect2::operator-(int minus) const
    {
        vect2 copy(*this);
    
        copy._container[0] -= minus;
        copy._container[1] -= minus;
        return (copy);
    }

    vect2	vect2::operator-(const vect2 minuson) const
    {
        vect2 copy(*this);

        copy._container[0] -= minuson._container[0];
        copy._container[1] -= minuson._container[1];
        return (copy);
    }


    vect2&	vect2::operator--(void)
    {
        this->_container[0] -= 1;
        this->_container[1] -= 1;
        return (*this);
    } 

    vect2	vect2::operator--(int)
    {
        vect2	copy(*this);

        this->_container[0] -= 1;
        this->_container[1] -= 1;
        return (copy);
    }


// multiplication OPERATORS
    vect2	vect2::operator*(int multi) const
    {
        vect2 copy(*this);
    
        copy._container[0] *= multi;
        copy._container[1] *= multi;
        return (copy);
    }

    vect2	vect2::operator*(const vect2 &multion) const
    {
        vect2 copy(*this);

        copy._container[0] *= multion._container[0];
        copy._container[1] *= multion._container[1];
        return (copy);
    }

    vect2	operator*(int number, const vect2& vector)
    {
        return (vector * number);
        //you dont need to create a second one because:
        // 1) it will infinte loop
        // 2) the "v2 * 3" thing is covered by the:
        //      vect2	vect2::operator*(int multi) const
    }


//Equal (==) OPERATORS
    vect2& vect2::operator=(const vect2& original)
    {
        if (this == &original)//need to add '&' because 'this' is a pointer
            return (*this);// handle self-assignment

        // add in the container value
        this->_container = original._container;
        return (*this);
    }

    bool	vect2::operator==(const vect2& other) const
    {
        if ((this->getContainer_elements(0) == other.getContainer_elements(0)) && 
            (this->getContainer_elements(1) == other.getContainer_elements(1)))
        {
            std::cout << "THE TRUTH RETURNED!--> " << std::flush;
            return (true); // handle self-assignment
        }
        return (false);
    }

    bool	vect2::operator!=(const vect2& other) const
    {
        if ((this->getContainer_elements(0) != other.getContainer_elements(0)) && 
            (this->getContainer_elements(1) != other.getContainer_elements(1)))
        {
            std::cout << "THE TRUTH RETURNED!--> " << std::flush;
            return (true); // handle self-assignment
        }
        return (false);
    }


//  all three plus equal (+/-/*)=
    vect2   vect2::operator+=(int add)
    {
        this->_container[0] += add;
        this->_container[1] += add;
        return (*this);
    }

    vect2   vect2::operator+=(const vect2 &addon)
    {
        this->_container[0] += addon.getContainer_elements(0);
        this->_container[1] += addon.getContainer_elements(1);
        return (*this);
    }

    vect2   vect2::operator-=(int minus)
    {
        this->_container[0] -= minus;
        this->_container[1] -= minus;

        return (*this);
    }

    vect2   vect2::operator-=(const vect2 &minuson)
    {
        this->_container[0] -= minuson.getContainer_elements(0);
        this->_container[1] -= minuson.getContainer_elements(1);
        return (*this);
    }

    vect2   vect2::operator*=(int multi)
    {
        this->_container[0] *= multi;
        this->_container[1] *= multi;

        return (*this);
    }

    vect2   vect2::operator*=(const vect2 &multion)
    {
        this->_container[0] *= multion.getContainer_elements(0);
        this->_container[1] *= multion.getContainer_elements(1);
        return (*this);
    }


//outside of class
std::ostream& operator<<(std::ostream& out, const vect2& v)
{
	out << "{" << v.getContainer_elements(0) << ", " << v.getContainer_elements(1) << "}";
	return (out);
}

#pragma once

#include <iostream>

template <typename T, size_t S>
class CircularSet
{
    //An array with the set's elements
    T m_values[S];
    //The indecies of the first an the last element in the set
    int m_first, m_last;

  public:

    CircularSet()
    {
        m_first = m_last = -1;
    }

    void Print() const
    {
        std::cout << "Printing set: " << std::endl;
        for (int i = m_first; true; i = (i + 1) % S)
        {
            std::cout << m_values[i] << std::endl;
            if (i == m_last)
            {
                break;
            }
        }
        std::cout << "--------------" << std::endl;
    }

    const T& operator[](int ind) const
    {
        if (!this->IndexIsValid(ind))
        {
            std::cout << "Error: Circular set index out of range (on [] operator)" << std::endl;
        }
        return m_values[this->GetArrayIndex(ind)];
    }

    size_t GetSize() const
    {
        if (m_first <= m_last)
            return m_last - m_first + 1;
        return S - (m_first - m_last - 1);
    }

    //Clears the set, makes it empty
    void Clear()
    {
        m_first = m_last = -1;
    }

    //Adds an element to the set
    void Add(T elem)
    {
        if (m_first == -1)
        {
            m_values[0] = elem;
            m_first = m_last = 0;
            return;
        }
        
        int afterLast = (m_last + 1) % S;
        if (afterLast == m_first)
        {
            std::cout << "Error: Circular set overflow" << std::endl;
            return;
        }

        m_values[afterLast] = elem;
        m_last = afterLast;
    }

    void RemoveAt(int ind)
    {
        if (m_first == -1)
        {
            std::cout << "Error: Cannot remove from an empty circular set" << std::endl;
            return;
        }
        if (!this->IndexIsValid(ind))
        {
            std::cout << "Error: Circular set index out of range (on remove)" << std::endl;
            return;
        }

        //If the element is the only element, the set becomes empty
        if (m_first == m_last)
        {
            m_first = m_last = -1;
            return;
        }

        int arrInd = this->GetArrayIndex(ind);

        //Swap the first element and the target element
        T tmp = m_values[m_first];
        m_values[m_first] = m_values[arrInd];
        m_values[arrInd] = tmp;

        //Remove the first element
        m_first = (m_first + 1) % S;
    }

  private:

    int GetArrayIndex(int listInd) const
    {
        return (m_first + listInd) % S;
    }

    bool IndexIsValid(int ind) const
    {
        int arrInd = this->GetArrayIndex(ind);
        return (
            (m_first <= m_last && arrInd >= m_first && arrInd <= m_last)
            ||
            (m_first > m_last && arrInd <= m_last)
        );
    }
};
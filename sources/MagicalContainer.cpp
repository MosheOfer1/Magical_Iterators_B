#include "MagicalContainer.hpp"
#include <algorithm>
namespace ariel
{
    MagicalContainer::MagicalContainer() {}
    MagicalContainer::~MagicalContainer() {}

    void MagicalContainer::addElement(int element)
    {
        auto it = std::lower_bound(elements.begin(), elements.end(), element);
        int index = std::distance(elements.begin(), it); // Determine the index where the new element will be inserted.
        elements.insert(it, element);
        updateIterators(index);
    }

    void MagicalContainer::removeElement(int element)
    {
        auto it = std::lower_bound(elements.begin(), elements.end(), element);

        if (it != elements.end() && *it == element)
        {
            elements.erase(it);
        }
        else
        {
            throw std::runtime_error("Element does not exist in the container");
        }
    }

    size_t MagicalContainer::size() const
    {
        return elements.size();
    }

    // Implementation of AscendingIterator
    MagicalContainer::AscendingIterator::AscendingIterator(MagicalContainer &container, bool end)
        : container(container), it(end ? container.elements.end() : container.elements.begin()), currentIndex(0)
    {
        container.registerIterator(this);
    }

    MagicalContainer::AscendingIterator::AscendingIterator(const AscendingIterator &other)
        : container(other.container), it(other.it), currentIndex(0)
    {
        container.registerIterator(this);
    }

    MagicalContainer::AscendingIterator::~AscendingIterator()
    {
        container.unregisterIterator(this);
    }

    MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator=(const AscendingIterator &other)
    {
        if (other.container != this->container)
        {
            throw std::runtime_error("Different containers");
        }
        
        it = other.it;
        return *this;
    }

    bool MagicalContainer::AscendingIterator::operator==(const AscendingIterator &other) const
    {
        return it == other.it;
    }

    bool MagicalContainer::AscendingIterator::operator!=(const AscendingIterator &other) const
    {
        return !(*this == other);
    }

    int MagicalContainer::AscendingIterator::operator*() const
    {
        if (it == container.elements.end())
        {
            throw std::out_of_range("Iterator is pointing to an invalid position");
        }
        return *it;
    }

    MagicalContainer::AscendingIterator &MagicalContainer::AscendingIterator::operator++()
    {
        if (*this == end())
        {
            throw std::runtime_error("Iterator Increment Beyond End");
        }
        
        ++it;
        ++currentIndex;
        return *this;
    }
    bool MagicalContainer::AscendingIterator::operator<(const AscendingIterator &other) const
    {
        return it < other.it;
    }

    bool MagicalContainer::AscendingIterator::operator>(const AscendingIterator &other) const
    {
        return it > other.it;
    }

    MagicalContainer::AscendingIterator MagicalContainer::AscendingIterator::begin()
    {
        return AscendingIterator(container);
    }
    MagicalContainer::AscendingIterator MagicalContainer::AscendingIterator::end()
    {
        return AscendingIterator(container, true);
    }
    void MagicalContainer::AscendingIterator::update(int index)
    {
        if (index <= currentIndex)
        {
            // If the new element was inserted before or at the current position,
            // increment the current index to keep pointing to the same element.
            ++currentIndex;
        }

        // Set the iterator to the current index.
        it = container.elements.begin() + currentIndex;
    }

    //
    MagicalContainer::SideCrossIterator::SideCrossIterator(MagicalContainer &container, bool end)
        : container(container), front_index(0), back_index(container.size() - 1), isFront(true)
    {
        container.registerIterator(this);
        if (end)
        {
            back_index = container.size() % 2 == 0 ? container.size() / 2 - 1 : (container.size() / 2);
            front_index = back_index + 1;
            // printf("Front: %zd, Back: %zd\n", front_index, back_index);
        }
    }
    MagicalContainer::SideCrossIterator::SideCrossIterator(const SideCrossIterator &other)
        : container(other.container), front_index(other.front_index), back_index(other.back_index), isFront(other.isFront)
    {
        container.registerIterator(this);
    }

    MagicalContainer::SideCrossIterator::~SideCrossIterator()
    {
        container.unregisterIterator(this);
    }

    MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator=(const SideCrossIterator &other)
    {
        if (other.container != this->container)
        {
            throw std::runtime_error("Different containers");
        }
        front_index = other.front_index;
        back_index = other.back_index;
        isFront = other.isFront;
        return *this;
    }

    bool MagicalContainer::SideCrossIterator::operator==(const SideCrossIterator &other) const
    {
        return front_index == other.front_index && back_index == other.back_index;
    }

    bool MagicalContainer::SideCrossIterator::operator!=(const SideCrossIterator &other) const
    {
        return !(*this == other);
    }

    int MagicalContainer::SideCrossIterator::operator*() const
    {
        if (isFront)
        {
            return container.elements[front_index];
        }
        else
        {
            return container.elements[back_index];
        }
    }

    MagicalContainer::SideCrossIterator &MagicalContainer::SideCrossIterator::operator++()
    {
        if (*this == end())
        {
            throw std::runtime_error("Iterator Increment Beyond End");
        }
        
        if (isFront)
        {
            front_index++;
        }
        else
        {
            back_index--;
        }
        isFront = !isFront; // Switch between front and back for the next iteration
        return *this;
    }

    bool MagicalContainer::SideCrossIterator::operator<(const SideCrossIterator &other) const
    {
        // Compare the dist between the indexes
        return (back_index - front_index) > (other.back_index - other.front_index);
    }
    bool MagicalContainer::SideCrossIterator::operator>(const SideCrossIterator &other) const
    {
        // It returns true if this iterator is later in the iteration order than 'other'.
        return !(*this < other) && (*this != other);
    }

    // Rest of SideCrossIterator methods...
    MagicalContainer::SideCrossIterator MagicalContainer::SideCrossIterator::begin()
    {
        return SideCrossIterator(container);
    }

    MagicalContainer::SideCrossIterator MagicalContainer::SideCrossIterator::end()
    {
        return SideCrossIterator(container, true);
    }

    void MagicalContainer::SideCrossIterator::update(int index)
    {
        // Define how SideCrossIterator should be updated
        if (container.size() % 2 == 0)
        {
            back_index = container.size() / 2 - 1;
            front_index = back_index + 1;
        }
        else
        {
            front_index = container.size() / 2;
            back_index = front_index;
        }
    }

    // Primes
    MagicalContainer::PrimeIterator::PrimeIterator(MagicalContainer &container, bool end)
        : container(container), index(0)
    {
        container.registerIterator(this);
        if (end)
        {
            index = container.size();
        }
        else
        {
            while (index < container.size() && !isPrime(container.elements[index]))
                ++index;
        }
    }
    MagicalContainer::PrimeIterator MagicalContainer::PrimeIterator::begin()
    {
        return PrimeIterator(container);
    }

    MagicalContainer::PrimeIterator MagicalContainer::PrimeIterator::end()
    {
        return PrimeIterator(container, true);
    }

    void MagicalContainer::PrimeIterator::update(int index)
    {
        if (index <= this->index)
        {
            // If the insertion happened before (or at) the current position, increment the index
            ++this->index;
        }
        // If the newly inserted number at 'index' is a prime number, and it's next in line
        if (index == this->index && isPrime(container.elements[(size_t)index]))
        {
            return; // The iterator is already at the correct position
        }
        // Otherwise, advance the iterator to the next prime number
        while (this->index < container.size() && !isPrime(container.elements[this->index]))
        {
            ++this->index;
        }
    }

    bool MagicalContainer::PrimeIterator::isPrime(int num)
    {
        if (num <= 1)
            return false;
        if (num <= 3)
            return true;
        if (num % 2 == 0 || num % 3 == 0)
            return false;
        for (int i = 5; i * i <= num; i += 6)
        {
            if (num % i == 0 || num % (i + 2) == 0)
                return false;
        }
        return true;
    }

    // Implementation of PrimeIterator

    MagicalContainer::PrimeIterator::PrimeIterator(const PrimeIterator &other)
        : container(other.container), index(other.index)
    {
        container.registerIterator(this);
    }

    MagicalContainer::PrimeIterator::~PrimeIterator()
    {
        container.unregisterIterator(this);
    }

    MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator=(const PrimeIterator &other)
    {
        if (other.container != this->container)
        {
            throw std::runtime_error("Different containers");
        }
        index = other.index;
        return *this;
    }

    bool MagicalContainer::PrimeIterator::operator==(const PrimeIterator &other) const
    {
        return index == other.index;
    }

    bool MagicalContainer::PrimeIterator::operator!=(const PrimeIterator &other) const
    {
        return !(*this == other);
    }
    bool MagicalContainer::PrimeIterator::operator<(const PrimeIterator &other) const
    {
        return index < other.index;
    }

    bool MagicalContainer::PrimeIterator::operator>(const PrimeIterator &other) const
    {
        return index > other.index;
    }

    int MagicalContainer::PrimeIterator::operator*() const
    {
        return container.elements[index];
    }

    MagicalContainer::PrimeIterator &MagicalContainer::PrimeIterator::operator++()
    {
        if (*this == end())
        {
            throw std::runtime_error("Iterator Increment Beyond End");
        }
        
        do
        {
            ++index;
        } while (index < container.size() && !isPrime(container.elements[index]));
        return *this;
    }
}
#pragma once

#include <vector>
#include <set>
#include <stdexcept>
namespace ariel
{
    class MagicalContainer
    {

    public:
        MagicalContainer();
        ~MagicalContainer();

        void addElement(int element);
        void removeElement(int element);
        size_t size() const;
        bool operator==(const MagicalContainer &other) const { return other.activeIterators == activeIterators && other.elements == elements; }
        bool operator!=(const MagicalContainer &other) const { return !(other == *this); }
        class AscendingIterator;
        class SideCrossIterator;
        class PrimeIterator;

        class Iterator
        {
        public:
            virtual ~Iterator() {}
            virtual void update(int index) = 0; // Define a pure virtual function to update iterators.
        };

    private:
        std::vector<int> elements;
        // Add a new set to store active Iterators.
        std::set<Iterator *> activeIterators;

        void updateIterators(int index)
        {
            for (auto *iter : activeIterators)
            {
                iter->update(index);
            }
        }
        void registerIterator(Iterator *iterator)
        {
            activeIterators.insert(iterator);
        }

        void unregisterIterator(Iterator *iterator)
        {
            activeIterators.erase(iterator);
        }
    };

    class MagicalContainer::AscendingIterator : public Iterator
    {
    public:
        AscendingIterator(MagicalContainer &container, bool end = false);
        AscendingIterator(const AscendingIterator &other);
        ~AscendingIterator();

        AscendingIterator &operator=(const AscendingIterator &other);
        bool operator==(const AscendingIterator &other) const;
        bool operator!=(const AscendingIterator &other) const;
        bool operator<(const AscendingIterator &other) const;
        bool operator>(const AscendingIterator &other) const;
        int operator*() const;
        AscendingIterator &operator++();

        AscendingIterator begin();
        AscendingIterator end();
        void update(int index) override;

    private:
        MagicalContainer &container;
        std::vector<int>::iterator it;
        int currentIndex;
    };

    class MagicalContainer::SideCrossIterator : public Iterator
    {
    public:
        SideCrossIterator(MagicalContainer &container, bool end = false);
        SideCrossIterator(const SideCrossIterator &other);
        ~SideCrossIterator();

        SideCrossIterator &operator=(const SideCrossIterator &other);
        bool operator==(const SideCrossIterator &other) const;
        bool operator!=(const SideCrossIterator &other) const;
        bool operator<(const SideCrossIterator &other) const;
        bool operator>(const SideCrossIterator &other) const;
        int operator*() const;
        SideCrossIterator &operator++();

        SideCrossIterator begin();
        SideCrossIterator end();
        void update(int index) override;

    private:
        MagicalContainer &container;
        size_t front_index;
        size_t back_index;
        bool isFront;
    };

    class MagicalContainer::PrimeIterator : public Iterator
    {
    public:
        PrimeIterator(MagicalContainer &container, bool end = false);
        PrimeIterator(const PrimeIterator &other);
        ~PrimeIterator();

        PrimeIterator &operator=(const PrimeIterator &other);
        bool operator==(const PrimeIterator &other) const;
        bool operator!=(const PrimeIterator &other) const;
        bool operator<(const PrimeIterator &other) const;
        bool operator>(const PrimeIterator &other) const;
        int operator*() const;
        PrimeIterator &operator++();

        PrimeIterator begin();
        PrimeIterator end();
        void update(int index) override;

    private:
        MagicalContainer &container;
        size_t index;

        bool isPrime(int num);
    };

} // namespace ariel

#ifndef MTM4_SET_H
#define MTM4_SET_H
#define END_OF_SET NULL

#include "exceptions.h"

namespace mtm{
    
    /**
     * A set
     * @tparam Type The type of an element on the group.
     *      Required to implement:
     *                             - operator ==
     *                             - copy constructor
     */
    template<typename Type>
    class MtmSet{

        /**
         * A node in the set
         */
        class Node{
			Type data;
			Node* next;
		public:

			Node(Type new_data, Node* next_node): data(Type(new_data)){
				this->next = next_node;
			}
			friend class iterator;
			friend class const_iterator;
			friend class MtmSet;
        };
	Node* header;
    public:
        //Forward declaration
        class const_iterator;
        
        /**
         * A iterator for Set
        */
        class iterator{     
		private:
			Node * current;
        public:
            /**
             * Empty constructor. Should not be dereferenced.
             * Same as MtmSet::end()
             */
            iterator(){
				this->current = END_OF_SET;
			}
            
            /**
             * Constructor of Set iterator
             * @param node The node the iterator points to
             */
            explicit iterator(Node *node){
				this->current = node;
			}
            
            /**
             * Copy constructor
             * @param it The iterator to copy
             */
            iterator(const iterator& it){
				this->current = it.current;
			}

            /**
             * Destructor
             */
            ~iterator() {}

            /**
             * Copy assignment operator.
             * @param rhs The iterator to copy.
             * @return A reference to this iterator.
             */
            iterator& operator=(const iterator& rhs){
				this->current = rhs.current;
				return *this;
			}
            
            /**
             * Dereference operator * .
             * Used like dereference of a pointer with * .
             * @return A reference of the data in the node the iterator
             *  points to.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type& operator*() const{
				if (current == END_OF_SET) {
					throw NodeIsEndException();
				}
				return current->data;
			
			}
            
            /**
             * Dereference operator -> .
             * Used like dereference of a pointer with -> .
             * @return A pointer to the data in the node the iterator
             *  points to.
             * C++ use the -> operator on the returned pointer,
             * this allows the user to treat the iterator like a pointer.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type *operator->() const{
				if (current == END_OF_SET) {
					throw NodeIsEndException();
				}
				return &(current->data);
			}
            
            /**
             * Prefix increment operator (++i)
             * @return a reference to the iterator;
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            iterator& operator++(){
				if (current == END_OF_SET) {
					throw NodeIsEndException();
				}
				current = current->next;
				return *this;
			}
            
            /**
             * Postfix increment operator (i++)
             * @return iterator that points to the same node as this before
             * the increment
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            iterator operator++(int){
				if (current == END_OF_SET) {
					throw NodeIsEndException();
				}
				iterator temp = *this;
				current = current->next;
				return temp;
			}
            
            /**
             * Compare an iterator with const_iterator. (when there is a
             *  regular iterator on the left.
             * @param rhs the right const_iterator.
             * @return true if the two iterators point to the same node
             */
            bool operator==(const const_iterator& rhs) const{
				if ((this->current==END_OF_SET)||(rhs.current==END_OF_SET))	{
					if (this->current == rhs.current)					{
						return true;
					}
					return false;
				}
				if (*(*this)==*rhs)	{
					return true;
				}
				return false;
			}
            
            /**
             * Compare an iterator with const_iterator. (when there is a
             *  regular iterator on the left.
             * @param rhs the right const_iterator.
             * @return true if the two iterators don't point to the same node
             */
            bool operator!=(const const_iterator& rhs) const{
				if (*this== rhs)			{
					return false;
				}
				return true;
			}
			friend class MtmSet;
            friend class const_iterator;
        };

        
        /**
         * A const_iterator for Set
        */
        class const_iterator{
			
			Node * current;
        public:

            /**
             * Empty constructor. Should not be dereferenced.
             * Same as MtmSet::end()
             */
            const_iterator(){
				current = END_OF_SET;
			}
            
            /**
             * Constructor of Set const_iterator
             * @param node The node the const_iterator points to
             */
            explicit const_iterator(Node *node){
				current = node;
			}
            
            /**
             * Copy constructor
             * @param it The const_iterator to copy
             */
            const_iterator(const const_iterator& it){
				this->current = it.current;
			}
            
            /**
             * Constructor from iterator (not const_iterator)
             * Allows casting from iterator to const_iterator
             * @param it The iterator to "copy" to a const_iterator
             */
            const_iterator(const iterator& it){
				this->current = it.current;
			}

            /**
             * Destructor
             */
            ~const_iterator(){}

            /**
             * Copy assignment operator.
             * @param rhs The iterator to copy.
             * @return A reference to this const_iterator.
             */
            const_iterator& operator=(const const_iterator& rhs){
				this->current = rhs.current;
				return *this;
			}
            
            /**
             * Dereference operator * .
             * Used like dereference of a pointer with * .
             * @return A reference of the data in the node the const_iterator
             *  points to.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type& operator*() const{
				if (current == END_OF_SET) {
					throw NodeIsEndException();
				}
				return current->data;
			}
            
            /**
             * Dereference operator -> .
             * Used like dereference of a pointer with -> .
             * @return A pointer to the data in the node the const_iterator
             *  points to.
             *  C++ use the -> operator on the returned pointer,
             *  this allows the user to treat the const_iterator like a pointer.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type *operator->() const{
				if (current == END_OF_SET) {
					throw NodeIsEndException();
				}
				return &(*this);			
			}
            
            /**
             * Prefix increment operator (++i)
             * @return a reference to the iterator;
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const_iterator& operator++(){
				if (current == END_OF_SET) {
					throw NodeIsEndException();
				}
				current = current->next;
				return *this;
			}
            
            /**
             * Postfix increment operator (i++)
             * @return const_iterator that points to the same node as this
             * before the increment
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const_iterator operator++(int){
				if (current == END_OF_SET) {
					throw NodeIsEndException();
				}
				const_iterator temp = *this;
				current = current->next;
				return temp;
			}
            
            /**
             * Compare two const_iterators.
             * @param rhs the right const_iterator
             * @return true if the two const_iterators point to the same node
             */
            bool operator==(const const_iterator& rhs) const{
				if (this->current == rhs.current) {
					return true;
				}
				return false;
			}
            
            /**
             * Compare two const_iterators.
             * @param rhs the right const_iterator
             * @return true if the two const_iterators don't point to the same
             * node
             */
            bool operator!=(const const_iterator& rhs) const{
				if (*this == rhs) {
					return false;
				}
				return true;
			}
			friend class iterator;
        };
		


        /**
         * Empty constructor
         * Creates an empty set
         */
        MtmSet(){
			this->header = END_OF_SET;
		}
        
        /**
         * Copy constructor
         * @param set the Set to copy
         */
        MtmSet(const MtmSet& set){
			if(set.header == END_OF_SET){
				this->header = END_OF_SET;
				return;
			}
			iterator temp_data(set.header);
			this->header = new Node(*temp_data, END_OF_SET);
			iterator temp_to_set(set.header);
			iterator temp_to_this(this->header);
			iterator end_of_set;
			temp_to_set++;
			while (temp_to_set != end_of_set)	{
				temp_to_this.current->next = new Node(*temp_to_set, END_OF_SET);
				temp_to_set++;
				temp_to_this++;
			}
		}
        
        /**
         * Destructor
         * Free all allocated memory in the set.
         */
        ~MtmSet(){
			this->clear();
		}
        
        /**
         * Insert a new element to the set, doesn't insert if there is already
         *  an equal element in the set.
         * If an element was't inserted, all iterators should stay valid.
         * @param elem The element to insert to the set
         * @return An iterator to the inserted element, or to the equal
         * element if the element wasn't inserted.
         */
        iterator insert(const Type& elem){
			iterator temp(header);
			iterator end_of_set;
			while (temp!= end_of_set) {
				if (*(temp) == elem) {
					return temp;
				}
				temp++;
			}
			 
			header = new Node(elem, this->header);
			iterator to_return(header);
			return to_return;
		}
        
        /**
         * Remove an element from the set. If there is no element equal to
         *  elem in the set, does nothing.
         * If an element wasn't removed, all iterators should stay valid.
         * @param elem the element to remove.
         */
        void erase(const Type& elem){
			if (this->empty()){
				return;
			}
			iterator temp(header);
			if (*temp == elem)	{
				Node* to_delete = header;
				header = header->next;
				delete to_delete;
				return;
			}
			iterator before_current=temp;
			while (temp.current != END_OF_SET) {
				
				if (*(temp) == elem) {
					before_current.current->next = temp.current->next;
					delete temp.current;
					break;
				}
				before_current = temp;
				temp++;
			}
		}
        
        /**
         * Remove the element the iterator points to from the set.
         * Iterator to another set is undefined.
         * @param it The iterator to the element to the set.
         */
        void erase(const iterator& it){
			erase(*it);
		}
        
        /**
         * Get the amount of elements in the set.
         * @return The amount of elements in the set.
         */
        int size() const{
			iterator temp(header);
			int size = 0;
			while (temp.current != END_OF_SET) {
				size++;
				temp++;
			}
			return size;
		}
        
        /**
         * Check if the set is empty.
         * @return true is the set is empty.
         */
        bool empty() const{
			return(this->size() == 0);
		}
        
        /**
         * Empty the set, free all allocated memory in the set.
         */
        void clear(){
			while (header != END_OF_SET) {
				iterator to_delete(header);
				erase(*(to_delete));
			}
		}
        
        /**
         * Get an iterator to the first element in the set.
         * If set is empty, return the same as end.
         * @return
         */
        iterator begin(){
			iterator temp(header);
			return temp;
		}
        
        /**
         * Get a const_iterator to the first element in the set.
         * If set is empty, return the same as end.
         * @return
         */
        const_iterator begin() const{
            const_iterator temp(header);
			return temp;
		}
        
        /**
         * Returns an iterator referring to the past-the-end element in the set.
         * The past-the-end element is the theoretical element that would follow
         * the last element in the set. It does not point to any element, and
         * thus should not be dereferenced.
         * @return Iterator to past-the-end element.
         */
        iterator end(){
			iterator temp;
			return temp;
		}
        
        /**
         * Returns a const_iterator referring to the past-the-end element in
         * the set.
         * The past-the-end element is the theoretical element that would follow
         * the last element in the set. It does not point to any element, and
         * thus should not be dereferenced.
         * @return const_iterator to past-the-end element.
         */
        const_iterator end() const{
			const_iterator temp;
			return temp;
		}
        
        /**
         * Find an element in the set.
         * @param elem The element to find
         * @return An iterator that points to the elem.
         */
        iterator find(const Type& elem){
			iterator temp(header);
			while (temp.current != END_OF_SET) {
				if (*(temp) == elem) {
					return temp;
				}
				temp++;
			}
			return temp;
		}
		
        
        /**
         * Find an element in the set.
         * @param elem The element to find
         * @return A const_iterator that points to the elem.
         */
        const_iterator find(const Type& elem) const{
				const_iterator temp(header);
				const_iterator end_of_set;
				while (temp != end_of_set) {
					if (*(temp) == elem) {
						return temp;
					}
					temp++;
				}
				return temp;
			}

        
        /**
         * Check if an element is in the set.
         * @param elem The element to check if it's in the set.
         * @return True if the element is in the set, false otherwise.
         */
		bool contains(const Type& elem) const {
			const_iterator temp = this->find(elem);
			const_iterator end_of_set;
			if (temp == end_of_set) {
				return false;
			}
			return true;
		}

        
        /**
         * Check if this set is a superset of a given set.
         * Superset: A superset of B if B is a subset of A, that mean's that
         *  A contains all the elements in B.
         * @param subset The set to check if it's a subset.
         * @return True if the given set is a subset of this set.
         */
        bool isSuperSetOf(const MtmSet& subset) const{
			iterator temp(subset.header);
			while (temp.current != END_OF_SET)	{
				if (this->contains(*temp) == false) {
					return false;
				}
				temp++;
			}
			return true;
		}
        
        /**
         * Check if two set are equal, meaning, they contain the same elements.
         * @param rhs The set right of the == operator.
         * @return true if thw two set conatain the same elements, false
         *  otherwise.
         */
        bool operator==(const MtmSet& rhs) const{
			if ((this->isSuperSetOf(rhs)==true)&&(this->size()==rhs.size())){
				return true;
			}
			return false;
		}
        
        /**
         * Check if two set are equal, meaning, they contain the same elements.
         * @param rhs The set right of the != operator.
         * @return false if thw two set conatain the same elements, true
         *  otherwise.
         */
        bool operator!=(const MtmSet& rhs) const{
			return (!(*this == rhs));
		}
        
        /**
         * Insert all the elements in the given set to this set (union).
         * @param set The set to insert all the elements from.
         * @return A reference to this set.
         */
        MtmSet& unite(const MtmSet& set){
			iterator temp(set.header);
			iterator end_of_set;
			if (*this == set)			{
				return *this;
			}
			while (temp != end_of_set) {
				this->insert(*temp);
				temp++;
			}
			return *this;
		}
        
        /**
         * Returns a new set that is an union of this set, and a given set.
         * @param set The other set (other than this) to be in the union.
         * @return The new set.
         */
        MtmSet unite(const MtmSet& set) const{
			MtmSet new_set(*this);
			new_set.unite(set);
			return new_set;
		}
        
        /**
         * Remove all the elements from this set, that are in the given set
         * (intersection).
         * @param set The other set in the intersection.
         * @return A reference to this set.
         */
        MtmSet& intersect(const MtmSet& set){
			if (*this == set) {
				return *this;
			}
			bool is_exist = false;
			iterator temp_this(header);
			while (temp_this.current != END_OF_SET)		{
				iterator temp_set(set.header);
				while (temp_set.current != END_OF_SET) {
					if (*temp_set==*temp_this)	{
						is_exist = true;
					}
					temp_set++;
				}
				iterator to_delete = temp_this;
				temp_this++;
				if (!is_exist)	{
					this->erase(to_delete);
					is_exist = false;
				}
			}
			return *this;
		}
        
        /**
         * Returns a new set that is an intersection of this set,
         *  and a given set.
         * @param set The other set (other than this) to be in the intersection.
         * @return The new set.
         */
        MtmSet intersect(const MtmSet& set) const{
			MtmSet new_set(*this);
			new_set.intersect(set);
			return new_set;
		}
        
        /**
         * Remove all the elements in the set, that doesn't meet a given
         *  condition.
         * @tparam func - A function of an object-function that receive 1
         *  argument, of the same type as an element in the set, and returns
         *  a bool.
         * @param condition - function (or object-function) that returns true
         * if the element is meeting the condition and false otherwise.
         * @return A reference to this set.
         */
        template<typename func>
        MtmSet& getSubSet(func condition){
			MtmSet temp(*this);
			iterator temp_iterator(temp.header);
			while (temp_iterator.current!= END_OF_SET){
				iterator to_delete = temp_iterator;
				temp_iterator++;
				if (condition(*(to_delete))==false) {
					this->erase(*(to_delete));
				}
			}
			return *this;
		}
        
        /**
         * Get a subset of this set, that contains all the elements in the set,
         * that doesn't meet a given condition.
         * @tparam func - A function of an object-function that receive 1
         *  argument, of the same type as an element in the set, and returns
         *  a bool.
         * @param condition - function (or object-function) that returns true
         * if the element is meeting the condition and false otherwise.
         * @return A the new set.
         */
        template<typename func>
        MtmSet getSubSet(func condition) const{
			MtmSet new_set(*this);
			new_set.getSubSet(condition);
			return new_set;
		}
    };
} // namespace mtm

#endif //MTM4_SET_H

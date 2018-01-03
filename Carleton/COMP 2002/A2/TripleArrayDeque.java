import java.util.NoSuchElementException;


public class TripleArrayDeque<T> implements SimpleList<T> {

	DoublyLinkedList<T> A, B, C;

	public TripleArrayDeque(){
		A = new DoublyLinkedList<T>();
		B = new DoublyLinkedList<T>();
		C = new DoublyLinkedList<T>();
	}

	private void balance(){
		/* rebalance the three lists whenever any of the lists have 3 times less
		 *element than the other two lists together. */
		 //priority is that after rebalancing, A.size()>=B.size()>=C.size()
		 //A gets the ceiling of size()/3; C, the floor; and B, whatever's left (ceiling or floor)

		 //(somewhat) SPECIAL CASES
		 //check for 0-0-1, 0-1-1 (in any order)
		 if((A.size()<=1) && (B.size()<=1) && (C.size()<=1))
		 	return;
		 //check for 1-2-2 (in any order)
		 if((size()==5) && (A.size()<=2) && (B.size()<=2) && (C.size()<=2))
		 	return;

		 if((3*A.size()<B.size()+C.size()) || (3*B.size()<A.size()+C.size()) || (3*C.size()<A.size()+B.size())){
			//move C's surplus over the floor of size()/3 to B
			while(C.size()>size()/3)
				B.addLast(C.removeFirst());

			//my own ceiling function, because I don't feel like importing one
			int ceil = size()/3+1;
			if(size()%3==0) //or if((3*(size()/3)-size()==0), to avoid % operator
				ceil = size()/3;
			//move A's surplus over the ceiling of size()/3 to B
			while(A.size()>ceil)
				B.addFirst(A.removeLast());

			//any surplus should now be in B, but A.size() and C.size() may NOT be similar in size
			//move B's surplus over size()-floor-ceiling to A and C
			while(B.size()>size()-size()/3-ceil){
				if(A.size()<ceil) //first move to A until it has its third
					A.addLast(B.removeFirst());
				else if(C.size()<size()/3) //then move to C until it has its third
					C.addFirst(B.removeLast());

			}
		 }
	}

	public void 	addFirst(T o)	{
		A.addFirst(o);
		balance();
	}

	public void 	addLast(T o)	{
		C.addLast(o);
		balance();
	}

	public void 	add(int index, T element){
		if((index<0) || (index>size()))
 			throw new IndexOutOfBoundsException();
 		else if(index==size())
 			C.addLast(element);
 		else if((index<=A.size()-1) || (index==0))
 			A.add(index, element);
 		else if(index>=A.size()+B.size())
 			C.add(index-A.size()-B.size(), element);
 		else B.add(index-A.size(), element);
 		balance();
	}

	public T 		getFirst()		{
		if(size()==0)
			throw new NoSuchElementException();
		if(A.size()>0)
			return A.getFirst();
		if(B.size()>0)
			return B.getFirst();
		return C.getFirst();
	}

	public T 		getLast()		{
		if(size()==0)
			throw new NoSuchElementException();
		if(C.size()>0)
			return C.getLast();
		if(B.size()>0)
			return B.getLast();
		return A.getLast();
	}

 	public T 		get(int index)	{
 		if((index<0) || (index>size()-1))
 			throw new IndexOutOfBoundsException();
 		if(index<=A.size()-1)
 			return A.get(index);
 		if(index>=A.size()+B.size())
 			return C.get(index-A.size()-B.size());
 		return B.get(index-A.size());
 	}


	public void 	clear()			{
		A = new DoublyLinkedList<T>();
		B = new DoublyLinkedList<T>();
		C = new DoublyLinkedList<T>();
	}

	public  T 		remove(int index){
		if((index<0) || (index>size()-1))
 			throw new IndexOutOfBoundsException();
 		T x;
		if(index<=A.size()-1)
 			x = A.remove(index);
 		else if(index>=A.size()+B.size())
 			x = C.remove(index-A.size()-B.size());
 		else x = B.remove(index-A.size());
 		balance();
 		return x;
	}

	public boolean 	remove(Object o){
		if(!A.remove(o)){
			if(!B.remove(o)){
				if(!C.remove(o))
					return false;
			}
		}
		balance();
		return true;
	}

	public T 		removeFirst()	{
		if(size()==0)
			throw new NoSuchElementException();
		T x;
		if(A.size()>0)
			x = A.removeFirst();
		else if(B.size()>0)
			x = B.removeFirst();
		else x = C.removeFirst();
		balance();
		return x;
	}

	public T 		removeLast()	{
		if(size()==0)
			throw new NoSuchElementException();
		T x;
		if(C.size()>0)
			x = C.removeLast();
		else if(B.size()>0)
			x = B.removeLast();
		else x = A.removeLast();
		balance();
		return x;
	}

	public  T 		set(int index, T element){
		if((index<0) || (index>size()-1))
 			throw new IndexOutOfBoundsException();
 		T x;
		if(index<=A.size()-1){
 			x = A.get(index);
 			A.set(index, element);
		}
 		else if(index>=A.size()+B.size()){
 			x = C.get(index-A.size()-B.size());
 			C.set(index-A.size()-B.size(), element);
 		}
 		else {
 			x = B.get(index-A.size());
 			B.set(index-A.size(), element);
 		}
 		return x;
	}

	public int 		size()			{
		return A.size() + B.size() + C.size();
	}

	public boolean 	contains(Object o){
		return ((A.contains(o)) || (B.contains(o)) || (C.contains(o)));
	}

	public int 		indexOf(Object o){
		int i = A.indexOf(o);
		if(i!=-1)
			return i;
		i = B.indexOf(o);
		if(i!=-1)
			return i+A.size();
		i = C.indexOf(o);
		if(i!=-1)
			return i+A.size()+B.size();
		return -1;
	}

	public String	toString()		{
		return A.toString() + B.toString() + C.toString();
	}
}

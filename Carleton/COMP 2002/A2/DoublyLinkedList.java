import java.util.NoSuchElementException;


public class DoublyLinkedList<T> implements SimpleList<T> {
	private class Node<T> {
		T x;
		Node<T> prev;
		Node<T> next;

		private Node(){
		}

		private Node(T o){
			x = o;
		}
	}

	DoublyLinkedList<T>.Node<T> dummy;
	int n;

	public DoublyLinkedList()		{
		dummy = this.new Node<T>();
		dummy.prev = dummy;
		dummy.next = dummy;
		n = 0;
	}

	private DoublyLinkedList<T>.Node<T>	getNode(int index){
		DoublyLinkedList<T>.Node<T> u;
		if(index<n/2){
			u = dummy.next;
			for(int j=0; j<index; j++, u=u.next){}
		}
		else {
			u = dummy.prev;
			for(int j=n-1; j>index; j--, u=u.prev){}
		}
		return u;
	}

	public void 	addFirst(T o)	{
		DoublyLinkedList<T>.Node<T> u = this.new Node<T>(o);
		u.prev = dummy;
		u.next = dummy.next;
		u.prev.next = u; //dummy.next = u;
		u.next.prev = u; //dummy.next.prev = u;
		n++;
	}

	public void 	addLast(T o)	{
		DoublyLinkedList<T>.Node<T> u = this.new Node<T>(o);
		u.next = dummy;
		u.prev = dummy.prev;
		u.prev.next = u; //dummy.prev.next = u;
		u.next.prev = u; //dummy.prev = u;
		n++;
	}

	public void 	add(int index, T element){
		if((index<0) || (index>n))
 			throw new IndexOutOfBoundsException();
 		if(index==n){
 			addLast(element);
 			return;
 		}
		DoublyLinkedList<T>.Node<T> u = getNode(index);
		DoublyLinkedList<T>.Node<T> v = this.new Node<T>(element);
		v.next = u;
		v.prev = u.prev;
		v.prev.next = v; //u.prev.next = v;
		v.next.prev = v; //u.prev = v;
		n++;
	}

	public T 		getFirst()		{
		if(n==0)
			throw new NoSuchElementException();
		return dummy.next.x; //null if empty
	}

	public T 		getLast()		{
		if(n==0)
			throw new NoSuchElementException();
		return dummy.prev.x; //null if empty
	}

 	public T 		get(int index)	{
 		if((index<0) || (index>n-1))
 			throw new IndexOutOfBoundsException();
 		return getNode(index).x;
 	}

	public void 	clear()			{ //basically this = new DoublyLinkedList<T>()
		dummy = this.new Node<T>();
		dummy.prev = dummy;
		dummy.next = dummy;
		n = 0;
	}

	public  T 		remove(int index){
		if((index<0) || (index>n-1))
 			throw new IndexOutOfBoundsException();
		DoublyLinkedList<T>.Node<T> u = getNode(index);
		u.prev.next = u.next;
		u.next.prev = u.prev;
		n--;
		return u.x;
	}

	public boolean 	remove(Object o){
		if(n==0){ //handle o==null so as not to decrement n if u==dummy
			return false;
		}
		DoublyLinkedList<T>.Node<T> u = dummy.next;
		while((u.next!=dummy) && (u.x!=o))
			u = u.next;
		if(u.x==o){ // && (u!=dummy), instead of if(n==0) above
			u.prev.next = u.next;
			u.next.prev = u.prev;
			n--;
			return true;
		}
		else return false;
	}

	public T 		removeFirst()	{
		if(n==0)
			throw new NoSuchElementException();;
		DoublyLinkedList<T>.Node<T> u = dummy.next;
		dummy.next = u.next; //dummy.next = dummy.next.next;
		dummy.next.prev = dummy; //u.next.prev = dummy;
		n--;
		return u.x;
	}

	public T 		removeLast()	{
		if(n==0)
			throw new NoSuchElementException();;
		DoublyLinkedList<T>.Node<T> u = dummy.prev;
		dummy.prev = u.prev; //dummy.prev = dummy.prev.prev;
		dummy.prev.next = dummy; //u.prev.next = dummy;
		n--;
		return u.x;
	}

	public  T 		set(int index, T element){
		if((index<0) || (index>n-1))
 			throw new IndexOutOfBoundsException();
 		DoublyLinkedList<T>.Node<T> u = getNode(index);
 		T y = u.x;
 		u.x = element;
 		return y;
	}

	public int 		size()			{
		return n;
	}

	public boolean 	contains(Object o){
		DoublyLinkedList<T>.Node<T> u = dummy.next;
		while((u.next!=dummy) && (u.x!=o))
			u = u.next;
		return u.x==o;
	}

	public int 		indexOf(Object o){
		int index = 0;
		DoublyLinkedList<T>.Node<T> u = dummy.next;
		while((u.x!=o) && (u.next!=dummy)){
			u = u.next;
			index ++;
		}
		if(u.x==o)
			return index;
		return -1;
	}

	public String	toString()		{
		String s = "[";
		Node<T> u = dummy.next;
		for(int i=0; i<n-1; i++){
			s += u.x.toString() + ", ";
			u = u.next;
		}
		if(u!=dummy)
			s += u.x.toString();
		s += "]";
		return s;
	}
}


;celkova suma cisel v poli
(defun mySum(lst)
	(if (null lst)
		0
		(+ (car lst) (mySum (cdr lst)))
	)
)

;pocet prvku v listu
(defun myLength (lst)
	(if (null lst)
		0
		(+ (myLength(cdr lst)) 1)
	)
)

;koncova rekurze
(defun myLengthTailCall (lst)
	(myLengthTailCallDoIt lst 0)
)
(defun myLengthTailCallDoIt (lst len)
	(if (null lst)
		len
		(myLengthTailCallDoIt (cdr lst) (+ 1 len))
	)
)
;find e in lst
(defun myHasElement (lst e)
	(if (null lst)
		nil
		(if (equal (car lst) e)
			T
			(myHasElement (cdr lst) e)
		)
	)
)
;vrat nty prvek v seznamu
(defun myNthElement (lst n)
	(myNthElementDoIt lst n 0)
)
(defun myNthElementDoIt (lst n curr) 
	(if (equal n curr)
		(car lst)
		(myNthElementDoIt (cdr lst) n (+ curr 1))
	)
)
;
(defun myMin (a b)
	(if (< a b)
		a
		b
	)
)
;
(defun myMinInList (lst)
	(if (null lst)
		nil
		(myMinInListDoIt (cdr lst) (car lst))
	)	
)
(defun myMinInListDoIt (lst min)
	(if (null lst)
		min
		(myMinInListDoIt (cdr lst) (if (< min (car lst)) min (car lst) )				)
	)
)
;jak udelat switch v lispu
(defun testSwitch (pts)
	(cond
		((>= pts 90)  "A")
		((>= pts 80)  "B")
		((>= pts 70)  "C")
		(T "F")
	)
)

(defun myRange (lo hi)
	(if (>= lo hi)
		nil
		(cons lo (myRange (+ lo 1) hi))
	)
)

(defun myReverseList (lst)
	(if (null lst)
		nil
		(cons (myreverseList (cdr lst)) (car lst))
	)
	//todo delete
)
; &optional -> explicitne definovany argument
(defun myReverse2 (lst &optional (acc nil))
	(if (null lst)
		acc
		(myreverse2 (cdr lst) (cons (car lst) acc))
	)
)

(defun myAppend (lst elem) 
	(if (null lst)
		(cons elem nil)
		(cons (car lst) (myAppend (cdr lst) elem))
	)
)
; todo go through Ninety-Nine lisp problems site
; nezvladne to velky cisla
(defun isPrime (n)
	(isPrimeImpl n 2)
)
(defun isPrimeImpl (n curr)
	(if (= n curr)
		T	
	(if (= (mod n curr) 0)
		nil
		(isPrimeImpl n (+ curr 1))
	)

	)
)

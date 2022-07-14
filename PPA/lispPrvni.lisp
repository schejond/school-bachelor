(defun sum ( x y )
	( + x y )
)
;normalni rekurze
(defun factorial (x)
	(if ( <= x 0) 
		1
		( * x (factorial ( - x 1 ) ) )		
	)
)
;koncova rekurze
(defun factorialEnd_doIt ( x acc )
	( if ( <= x 1 ) 
		acc
		(factorialEnd_doIt (- x 1) (* x acc))
	)
)

(defun factorialEnd ( x )
	(factorialEnd_doIt x 1)
)

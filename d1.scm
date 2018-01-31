;; this is how to load external modules in scheme
(load-from-path "/afs/nd.edu/user37/cmc/Public/paradigms/scheme/d1/paradigms_d1.scm")
(use-modules (ice-9 paradigms_d1))

;; Ryan Loizzo

;; the list q
;; notice it has a ' in front of the list; that tells the interpreter to read
;; the list literally (e.g., as atoms, instead of functions)
(define q '(turkey (gravy) (stuffing potatoes ham) peas))

;; question 1
(display "question 1: ")
(display (atom? (car (cdr (cdr q)))))
(display "\n")
;; output:
;; #f
;;
;; explanation:
;; The innermost cdr returns everything after the first atom ((gravy) (stuffing potatoes ham) peas)
;; The next cdr returns ((stuffing potatoes ham) peas)
;; The car returns the first atom (stuffing potatoes ham)
;; This is a list of atoms and not a single atom, so false is printed

;; question 2
(display "question 2: ")
(display (lat? (car (cdr (cdr q)))))
(display "\n")
;; output:
;; #t
;;
;; explanation:
;; The three innermost functions will again return (stuffing potatoes ham). 
;; This is a list of atoms (lat), so true is printed


;; question 3
(display "question 3: ")
(display (cond ((atom? (car q)) (car q)) (else '())))
(display "\n")
;; output:
;; turkey
;;
;; explanation:
;; The condition checks if (car q) is an atom. 
;; Since this is true, car q is printed, which is turkey. 
;; If the conditional was false, it would go to the else which would print nothing.


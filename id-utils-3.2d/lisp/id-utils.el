;;; id-utils.el -- emacs interface to `lid -R grep', a.k.a. `gid'
;;; Copyright (C) 1995, 1996 Free Software Foundation, Inc.
;;; Greg McGary <gkm@gnu.ai.mit.edu>.

;; This file is part of GNU id-utils.

;; GNU id-utils is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.

;; GNU id-utils is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs; see the file COPYING.  If not, write to the
;; Free Software Foundation, 59 Temple Place - Suite 330, Boston,
;; MA 02111-1307, USA.

;;; This package provides the tools meant to help editing PO files,
;;; as documented in the GNU id-utils user's manual.  See this manual
;;; for user documentation, which is not repeated here.

;;; To install, merely put this file somewhere GNU Emacs will find it,
;;; then add the following lines to your .emacs file:
;;;
;;;   (autoload 'gid "id-utils")
;;;
;;; You may also adjust some customizations variables, below, by defining
;;; them in your .emacs file.

(require 'compile)
(provide 'id-utils)

(defvar gid-command "gid" "The command run by the gid function.")

(defun gid (args)
  "Run gid, with user-specified ARGS, and collect output in a buffer.
While gid runs asynchronously, you can use the \\[next-error] command to
find the text that gid hits refer to. The command actually run is
defined by the gid-command variable."
  (interactive (list (read-input
     (concat "Run " gid-command " (with args): ") (word-around-point))))
  (let (compile-command
	(compilation-error-regexp-alist grep-regexp-alist)
	(compilation-buffer-name-function '(lambda (mode)
					     (concat "*" gid-command " " args "*"))))
    ;; For portability to v19, use compile rather than compile-internal.
    (compile (concat gid-command " " args))))

(defun word-around-point ()
  "Return the word around the point as a string."
  (save-excursion
    (if (not (eobp))
	(forward-char 1))
    (forward-word -1)
    (forward-word 1)
    (forward-sexp -1)
    (buffer-substring (point) (progn
				(forward-sexp 1)
				(point)))))

":"; exec emacs -batch -l "$0" -- "$@"
;;; create-changelog.el

;; Copyright (C) 2010 Free Software Foundation, Inc.
;;
;; This program is free software, licensed under the terms of the GNU
;; General Public License as published by the Free Software Foundation,
;; either version 3 of the License, or (at your option) any later version.
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

;;; Author: Thien-Thi Nguyen

;;; Commentary:

;; Usage: create-changelog.el [--since START] [FILENAME]
;;
;; Write a suitable ChangeLog made from "git log" output to FILENAME.
;; If FILENAME is not specified, write to standard output.
;;
;; Normally, process all log entries (since the beginning of the repo)
;; to the present.  Optional arg `--since START' specifies a different
;; start point.  START can be an ISO date (YYYY-MM-DD) or a reference
;; (branch or tag).
;;
;; Regardless of the selected range, entries whose "subject line"
;; ends with "; nfc." are omitted unconditionally.

;;; Code:

(require 'cl)

(defun fso (s &rest args)
  (princ (apply 'format s args)))

(defvar version "1.0")

(defvar outfile nil)
(defvar which nil)

(let ((ls (cdr command-line-args-left))
      arg)
  (while (setq arg (pop ls))
    (case (and (not (zerop (length arg)))
               (intern arg))
      ((--version)
       (fso "create-changelog %s\n" version)
       (kill-emacs))
      ((--help)
       (with-temp-buffer
         (insert-file-contents load-file-name)
         (search-forward "Usage: ")
         (beginning-of-line)
         (while (looking-at ";; \\{0,1\\}")
           (fso "%s\n" (buffer-substring (match-end 0) (line-end-position)))
           (forward-line 1)))
       (kill-emacs))
      ((--since)
       (setq arg (pop ls)
             which (format (if (string-match
                                "[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]"
                                arg)
                               "--since=%s"
                             "%s..")
                           arg)))
      (t
       (when (string-match "^--" arg)
         (error "ERROR: Unrecognized option: `%s'" arg))
       (setq outfile arg)))))

(fso "Generating %s\n" (or which "full history"))

(setq vc-handled-backends nil)

(buffer-disable-undo)
(erase-buffer)

(save-excursion
  (let ((args `("--no-merges"
                "--format=&B%n%ci%s%n  %an  <%ae>%n%n\t%s%n%b%n&E%n"
                ,@(when which
                    (list which)))))
    (apply 'call-process "git" nil t nil "log" args)))

(let (p (q (make-marker)) r)
  (while (setq p (search-forward "&B\n" nil t))
    (set-marker q (save-excursion (search-forward "\n&E")))
    (if (looking-at ".*; nfc.$")
        (delete-region p q)
      (delete-region (+ p 10) (1+ (line-end-position)))
      (forward-line 3)
      (setq p (point))
      (when (setq r (re-search-forward "^*" q t))
        (delete-region p (1- r))
        (forward-char -1))
      (while (< (point) (- q 3))
        (insert "\t")
        (forward-line 1)))))

(goto-char (point-min))
(delete-trailing-whitespace)
(flush-lines "^&[BE]")
(while (re-search-forward "\\(\n\n\\)\n+" nil t)
  (replace-match "\\1"))

(if outfile
    (write-file outfile)
  (fso "----------\n%s\n" (buffer-string)))

(kill-emacs)

;;; create-changelog.el ends here

(cl:in-package :asdf)

(defsystem "rplidar_ros-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "tilter" :depends-on ("_package_tilter"))
    (:file "_package_tilter" :depends-on ("_package"))
  ))
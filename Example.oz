functor
import
   JSON at 'x-ozlib://doublec/json/json.so{native}'
   Application
   System
define
   X = {JSON.encode json(one:"1"
			 two:2
			 three:[1000 2000 3000])}
   {System.showInfo X}
   {System.show {JSON.decode X}}
in
   {Application.exit 0}
end

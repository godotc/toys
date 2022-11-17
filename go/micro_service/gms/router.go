package gms

import (
	"fmt"
	"log"
	"net/http"
	"strings"
)

type HandleFunc func(w http.ResponseWriter, r *http.Request)

type routerGroup struct {
	name            string
	handleFuncMap   map[string]HandleFunc
	handleMethodMap map[string][]string
}

func (r *routerGroup) Add(name string, method string, function HandleFunc) {
	r.handleFuncMap[name] = function
	r.handleMethodMap[method] = append(r.handleMethodMap[method], name)
}

func (r *routerGroup) Any(name string, function HandleFunc) {
	r.Add(name, http.MethodGet, function)
}

func (r *routerGroup) Get(name string, function HandleFunc) {
	r.Add(name, http.MethodGet, function)
}

func (r *routerGroup) Post(name string, function HandleFunc) {
	r.Add(name, http.MethodPost, function)
}

// ---------------------------------------router
// ! /user/getuser/...
// ! /message/new/....
type router struct {
	routerGroups []*routerGroup
}


func (r *router) NewGnoup(name string) *routerGroup {
	group := &routerGroup{
		name:            name,
		handleFuncMap:   make(map[string]HandleFunc),
		handleMethodMap: make(map[string][]string),
	}
	r.routerGroups = append(r.routerGroups, group)

	return group
}

// ---------------------------------------Engine
type Engine struct {
	router
}

func New() *Engine {
	return &Engine{}
}

func (e *Engine) ServeHTTP(w http.ResponseWriter, r *http.Request) {

	log.Printf(r.RequestURI)

	method := r.Method
	log.Println("method: ", method)
	//args := strings.Split(r.RequestURI, "/")
	//log.Println("args: ", args)
	group_name := strings.Split(r.RequestURI, "/")[1]
	//log.Println("groupName: ", group_name)

	for _, group := range e.routerGroups {
		// 如果是当前组 /user /order
		if group.name == group_name {

			for name, handle_method := range group.handleFuncMap {

				url := "/" + group_name + name

				// handle func map 内有这个 url
				if r.RequestURI == url {
					// 判断 handle method 内 有没有 这个 url

					//! 如果属于 ANY 直接 invoke
					routers, ok := group.handleMethodMap["ANY"]
					if ok {
						for _, routerName := range routers {
							if routerName == name {
								handle_method(w, r)
								return
							}
						}
					}

					//! 否则按method 进行匹配
					routers, ok = group.handleMethodMap[method]

					if ok {
						// 当前method下 的 router中有没有相应的 /<group>/name
						for _, routerName := range routers {
							if routerName == name {
								handle_method(w, r)
								return
							}
						}
					}

					w.WriteHeader(http.StatusMethodNotAllowed)
					fmt.Fprintln(w, r.RequestURI, method, "not allowed ")
					return
				}
			}

		}
	}

	w.WriteHeader(http.StatusNotFound)
	fmt.Fprintf(w, "%s not found.\n", r.RequestURI)
}

func (e *Engine) Run() {
	http.Handle("/", e)

	err := http.ListenAndServe(":9527", nil)
	if err != nil {
		log.Fatal(err)
	}
}

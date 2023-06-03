add_requires("cuda")


target("main")
	add_files("src/main.cc")
	add_files("src/vec_add.cu")
	add_packages("cuda")

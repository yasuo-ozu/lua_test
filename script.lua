
-- func() defined in c
func()
_G['func']()

-- func_return_table() defined in c
test_table = func_return_table()
print(test_table.greeting)
test_table.f()

-- global func may be deleted
func = "deleted func()"
print(func)

-- define user function (called from c)
function luafunc(a, b)
	-- type will be identified in c
	return a * a * b
	-- return "bad func"
end

-- error
function erroc()
	-- error("myerror")
end
erroc()

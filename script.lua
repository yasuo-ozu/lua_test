
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
	-- return type will be identified in c
	return a * a * b
	-- return "bad func"
end

-- error
function erroc()
	-- error("myerror")
end
erroc()

-- coroutine test (iterated from c)
function test_coroutine()
	for i=1, 10 do
		coroutine.yield(i * i)
	end
	return 1
end

-- coroutine can also be called from lua
co = coroutine.create(test_coroutine)
repeat
	f, a = coroutine.resume(co)
	print("YIELD_LUA: " .. a)
until coroutine.status(co) == "dead"



example%: main%.cpp
	$(CXX) -o $@ $^ -lpthread

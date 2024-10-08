example%: main%.cpp
	$(CXX) -o $@ $^ -lpthread

opt-example%: main%.cpp
	$(CXX) -O2 -o $@ $^ -lpthread

example%: main%.cpp
	$(CXX) -o $@ $^ -lpthread

opt-example%: main%.cpp
	$(CXX) -O3 -o $@ $^ -lpthread

san-example%: main%.cpp
	$(CXX) -fsanitize=thread -o $@ $^ -lpthread

real-san-example%: main%.cpp
	$(CXX) -g -fsanitize=thread -o $@ $^ -lpthread

clean:
	$(RM) example* opt-example* san-example* real-san-example*

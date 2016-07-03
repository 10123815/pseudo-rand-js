#include <random>
#include <cmath>
#include <node.h>

using namespace v8;

bool Check(const FunctionCallbackInfo<Value>& args, const uint8_t count)
{

	Isolate* isolate = args.GetIsolate();

	// Check the number of argiments passed.
	if (args.Length() != count)
	{
		isolate->ThrowException(Exception::Error(
		                            String::NewFromUtf8(isolate, "Wrong number of arguments")));
		return false;
	}

	for (int i = 0; i < count; ++i)
	{
		if (!args[i]->IsNumber())
		{
			isolate->ThrowException(Exception::TypeError(
			                            String::NewFromUtf8(isolate, "Wrong types of arguments")));
			return false;
		}
	}

	return true;
}

// Return a integer follow a uniform distribution.
// @param[in]	args[0]		Min value.
// @param[in]	args[1] 	Max value.
// @param[out]	args		Random value.
void UniformInt(const FunctionCallbackInfo<Value>& args)
{
	if (!Check(args, 2))
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	int min_v = args[0]->NumberValue();
	int max_v = args[1]->NumberValue();
	std::uniform_int_distribution<> dis(min_v, max_v);
	args.GetReturnValue().Set(dis(gen));

}

// Return a float follow a uniform distribution.
// @param[in]	args[0]		Min value.
// @param[in]	args[1] 	Max value.
// @param[out]	args		Random value.
void UniformReal(const FunctionCallbackInfo<Value>& args)
{
	if (!Check(args, 2))
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	double min_v = args[0]->NumberValue();
	double max_v = args[1]->NumberValue();
	std::uniform_real_distribution<> dis(min_v, max_v);
	args.GetReturnValue().Set(dis(gen));

}

// Return a float follow a geometric distribution.
// @param[in]	args[0]		Reverse of excepted.
// @param[out]	args		Random value.
void Geometric(const FunctionCallbackInfo<Value>& args)
{

	if (!Check(args, 1))
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	double p = args[0]->NumberValue();
	std::geometric_distribution<> dis(p);
	args.GetReturnValue().Set(dis(gen));

}

// Return a float follow a exponential distribution.
// @param[in]	args[0]		Lambda.
// @param[out]	args		Random value.
void Exponential(const FunctionCallbackInfo<Value>& args)
{

	if (!Check(args, 1))
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	double lambda = args[0]->NumberValue();
	std::exponential_distribution<> dis(lambda);
	args.GetReturnValue().Set(dis(gen));

}

// Return a float follow a normal distribution.
// @param[in]	args[0]		Mean.
// @param[in] 	args[1] 	Standard devision.
// @param[out]	args		Random value.
void Normal(const FunctionCallbackInfo<Value>& args)
{

	if (!Check(args, 2))
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	double m = args[0]->NumberValue();
	double d = args[1]->NumberValue();
	std::normal_distribution<> dis(m, d);
	args.GetReturnValue().Set(dis(gen));

}

// Return a float follow a proactive normal distribution.
// @param[in]	args[0]		Mean.
// @param[in] 	args[1] 	Standard devision.
// @param[out]	args		Random value.
void ProactiveExpNormal(const FunctionCallbackInfo<Value>& args)
{

	if (!Check(args, 2))
		return;

	std::random_device rd;
	std::mt19937 gen(rd());
	double ml = args[0]->NumberValue();
	double dl = args[1]->NumberValue();
	std::exponential_distribution<> dis_m(ml);
	std::exponential_distribution<> dis_d(dl);
	double m = dis_m(gen);
	double d = dis_d(gen);

	std::normal_distribution<> dis(m, d);
	double res = std::log(dis(gen));
	res = (res < m) ? 2 * m - res : res;
	args.GetReturnValue().Set(res);

}

void init(Local<Object> exports)
{
	NODE_SET_METHOD(exports, "uniInt", UniformInt);
	NODE_SET_METHOD(exports, "uniReal", UniformReal);
	NODE_SET_METHOD(exports, "geo", Geometric);
	NODE_SET_METHOD(exports, "exp", Exponential);
	NODE_SET_METHOD(exports, "norm", Normal);
	NODE_SET_METHOD(exports, "pnorm", ProactiveExpNormal);
}

NODE_MODULE(pseudo_rand, init);
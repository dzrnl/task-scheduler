#include <lib/notstd/any.h>
#include <lib/notstd/function.h>
#include <lib/notstd/optional.h>

#include <vector>

class TaskScheduler {
    template<typename Arg>
    struct result_type;

    class TaskBase {
      public:
        virtual notstd::any getResult() = 0;

        virtual ~TaskBase() = default;
    };

    template<typename... Args>
    class Task : public TaskBase {
      public:
        template<typename Callable>
        explicit Task(Callable callable, Args... args) : function_(callable), arguments_(std::make_tuple(args...)) {}

        notstd::any getResult() override {
            if (!result_.has_value()) {
                result_ = std::apply(function_, arguments_);
            }

            return result_.value();
        }

      private:
        notstd::function<notstd::any(typename result_type<Args>::type...)> function_;
        std::tuple<Args...> arguments_;
        notstd::optional<notstd::any> result_;
    };

    template<typename R, typename... Args>
    class Result {
      public:
        explicit Result(Task<Args...>& task) : task_(task) {}

        operator R() {
            return task_.getResult().template cast<R>();
        }

      private:
        Task<Args...>& task_;
    };

    template<typename Arg>
    struct result_type {
        using type = Arg;
    };

    template<typename R, typename... Args>
    struct result_type<Result<R, Args...>> {
        using type = R;
    };

  public:
    template<typename Callable, typename... Args>
    Task<Args...>& add(Callable callable, Args... args) {
        tasks_.emplace_back(std::make_unique<Task<Args...>>(callable, args...));

        return dynamic_cast<Task<Args...>&>(*tasks_.back());
    }

    template<typename R, typename... Args>
    Result<R, Args...> getFutureResult(Task<Args...>& task) {
        return Result<R, Args...>(task);
    }

    template<typename R, typename... Args>
    R getResult(Task<Args...>& task) {
        return task.getResult().template cast<R>();
    }

    void executeAll() {
        for (std::unique_ptr<TaskBase>& task : tasks_) {
            task->getResult();
        }
    }

  private:
    std::vector<std::unique_ptr<TaskBase>> tasks_;
};

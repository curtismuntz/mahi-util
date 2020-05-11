#include <Mahi/Util/Coroutine.hpp>
#include <chrono>

namespace mahi {
namespace util {

//==============================================================================
// YieldInstruction
//==============================================================================

YieldInstruction::YieldInstruction() {
}

YieldInstruction::~YieldInstruction() {
}

bool YieldInstruction::is_over() {
   return true;
}

YieldTime::YieldTime(Time duration) :
   m_dur(duration)
{
    m_clk.restart();
}

bool YieldTime::is_over() {
    return m_clk.get_elapsed_time() >= m_dur;
}

YieldUntil::YieldUntil(std::function<bool()> func) :
    m_func(func)
{
    
}

bool YieldUntil::is_over() {
    return m_func();
}

YieldWhile::YieldWhile(std::function<bool()> func) :
    m_func(func)
{
    
}

bool YieldWhile::is_over() {
    return !m_func();
}


//==============================================================================
// PromiseType
//==============================================================================

PromiseType::PromiseType() :
   m_instruction(nullptr)
{
}

PromiseType::~PromiseType() {
}

SuspendAlways PromiseType::initial_suspend() {
   return SuspendAlways{}; // changing this to change start
}

SuspendAlways PromiseType::final_suspend() {
   return SuspendAlways{};
}

Enumerator PromiseType::get_return_object() {
    auto h = std::coroutine_handle<PromiseType>::from_promise(*this);
    std::shared_ptr<Coroutine> coro(new Coroutine(h));
    return Enumerator(coro);
}

void PromiseType::unhandled_exception() {
   std::exit(1);
}

SuspendNever PromiseType::return_void() {
   return SuspendNever{};
}

SuspendAlways PromiseType::yield_value(YieldInstruction* value) {
   m_instruction = std::shared_ptr<YieldInstruction>(value);
   return SuspendAlways{};
}

SuspendAlways PromiseType::yield_value(std::shared_ptr<YieldInstruction>&& value) {
    m_instruction = std::move(value);
    return SuspendAlways{};
}

//==============================================================================
// Coroutine
//==============================================================================

Coroutine::Coroutine(std::coroutine_handle<PromiseType> coroutine) :
    YieldInstruction(),
    m_coroutine(coroutine),
    m_stop(false)
{
}

Coroutine::Coroutine(Coroutine &&other) :
    m_coroutine(other.m_coroutine)
{
    other.m_coroutine = nullptr;
}

Coroutine::~Coroutine() {
    if (m_coroutine) {
        m_coroutine.destroy();
    }
}

bool Coroutine::is_over() {
    return m_coroutine.done();
}

void Coroutine::stop() {
    m_stop = true;
}

//==============================================================================
// Enumerator
//==============================================================================

bool Enumerator::step() {
    if (m_ptr->m_stop) // coroutine has request stop
        return false;
    auto& instruction = m_ptr->m_coroutine.promise().m_instruction;
    if (instruction) { // there is an instruction
        if (instruction->is_over()) { // yield instruction is over
            m_ptr->m_coroutine.resume();
            return !m_ptr->m_coroutine.done();
        }
        else { // yield instruction is not over
            return true;
        }
    }
    else { // no yield instruction
        m_ptr->m_coroutine.resume();
        return !m_ptr->m_coroutine.done();
    }
}

Enumerator::Enumerator(std::shared_ptr<Coroutine> h) :
    m_ptr(h)
{
}

Enumerator::~Enumerator() {

}

Enumerator::Enumerator(Enumerator &&e) :
    m_ptr(std::move(e.m_ptr))
{
    e.m_ptr = nullptr;
};

std::shared_ptr<Coroutine> Enumerator::get_coroutine() {
    return m_ptr;
}

//==============================================================================
// Coroutine Manager
//==============================================================================

std::shared_ptr<util::Coroutine> CoroutineManager::start(util::Enumerator&& e) {
    auto h = e.get_coroutine();
    m_coroutines.push_back(std::move(e));
    return h;
}

void CoroutineManager::stop(std::shared_ptr<util::Coroutine> routine) {
    if (routine)
        routine->stop();
}

void CoroutineManager::stop_all() { m_coroutines.clear(); }

int CoroutineManager::count() const { return static_cast<int>(m_coroutines.size()); }

void CoroutineManager::step_all() {
    if (!m_coroutines.empty())
    {
        std::vector<util::Enumerator> temp;
        temp.swap(m_coroutines);
        for (auto &coro : temp)
        {
            if (coro.step())
                m_coroutines.push_back(std::move(coro));
        }
    }
}

} // namespace util
} // namespac mahi


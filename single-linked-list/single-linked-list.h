#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
  struct Node {
    Node() = default;
    Node(const Type& val, Node* next) : value(val), next_node(next) {}
    Type value{};
    Node* next_node = nullptr;
  };

  template <typename ValueType>
  class BasicIterator {
    friend class SingleLinkedList;
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Type;
    using difference_type = std::ptrdiff_t;
    using pointer = ValueType*;
    using reference = ValueType&;

    BasicIterator();
    explicit BasicIterator(Node* node);
    BasicIterator(const BasicIterator<Type>& another) noexcept;

    BasicIterator& operator=(const BasicIterator& rhs);

    [[nodiscard]] bool operator==(
        const BasicIterator<const Type>& rhs) const noexcept;
    [[nodiscard]] bool operator==(
        const BasicIterator<Type>& rhs) const noexcept;

    [[nodiscard]] bool operator!=(
        const BasicIterator<const Type>& rhs) const noexcept;
    [[nodiscard]] bool operator!=(
        const BasicIterator<Type>& rhs) const noexcept;

    BasicIterator& operator++() noexcept;
    BasicIterator operator++(int) noexcept;

    [[nodiscard]] reference operator*() const noexcept;
    [[nodiscard]] pointer operator->() const noexcept;

   private:
    Node* node_ = nullptr;
  };

 public:
  using value_type = Type;
  using reference = value_type&;
  using const_reference = const value_type&;
  using Iterator = BasicIterator<Type>;
  using ConstIterator = BasicIterator<const Type>;

  SingleLinkedList();

  SingleLinkedList(std::initializer_list<Type> values);
  SingleLinkedList(const SingleLinkedList& another);
  ~SingleLinkedList();

  SingleLinkedList& operator=(const SingleLinkedList& rhs);

  void swap(SingleLinkedList& another) noexcept;

  [[nodiscard]] Iterator begin() noexcept;
  [[nodiscard]] Iterator end() noexcept;

  [[nodiscard]] ConstIterator begin() const noexcept;
  [[nodiscard]] ConstIterator end() const noexcept;
  [[nodiscard]] ConstIterator cbegin() const noexcept;
  [[nodiscard]] ConstIterator cend() const noexcept;

  [[nodiscard]] Iterator before_begin() noexcept;

  [[nodiscard]] ConstIterator before_begin() const noexcept;
  [[nodiscard]] ConstIterator cbefore_begin() const noexcept;

  [[nodiscard]] size_t GetSize() const noexcept;
  [[nodiscard]] bool IsEmpty() const noexcept;

  void PushFront(const Type& value);
  void PopFront() noexcept;

  Iterator InsertAfter(ConstIterator pos, const Type& value);
  Iterator EraseAfter(ConstIterator pos) noexcept;

  void Clear() noexcept;

 private:
  Node head_;
  size_t size_ = 0ull;

  template <typename T>
  void Initialize(T begin, T end, size_t size);
};

template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>::BasicIterator() = default;

template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>::BasicIterator(Node* node)
    : node_(node) {}

template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>::BasicIterator(
    const BasicIterator<Type>& another) noexcept
    : node_(another.node_) {}

template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>& SingleLinkedList<Type>::BasicIterator<ValueType>::operator=(
    const BasicIterator& rhs) = default;

template <typename Type>
template <typename ValueType>
[[nodiscard]] bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator==(
    const BasicIterator<const Type>& rhs) const noexcept {
  return node_ == rhs.node_;
}

template <typename Type>
template <typename ValueType>
[[nodiscard]] bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator!=(
    const BasicIterator<const Type>& rhs) const noexcept {
  return !(*this == rhs);
}

template <typename Type>
template <typename ValueType>
[[nodiscard]] bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator==(
    const BasicIterator<Type>& rhs) const noexcept {
  return node_ == rhs.node_;
}

template <typename Type>
template <typename ValueType>
[[nodiscard]] bool SingleLinkedList<Type>::BasicIterator<ValueType>::operator!=(
    const BasicIterator<Type>& rhs) const noexcept {
  return !(*this == rhs);
}

template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>&
SingleLinkedList<Type>::BasicIterator<ValueType>::operator++() noexcept {
  assert(node_ != nullptr);
  node_ = node_->next_node;
  return *this;
}

template <typename Type>
template <typename ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>
SingleLinkedList<Type>::BasicIterator<ValueType>::operator++(int) noexcept {
  auto old_value(*this);
  ++(*this);
  return old_value;
}

template <typename Type>
template <typename ValueType>
ValueType& SingleLinkedList<Type>::BasicIterator<ValueType>::operator*() const noexcept {
  assert(node_ != nullptr);
  return node_->value;
}

template <typename Type>
template <typename ValueType>
ValueType* SingleLinkedList<Type>::BasicIterator<ValueType>::operator->() const noexcept {
  assert(node_ != nullptr);
  return &node_->value;
}
//--start of single list methods--
template <typename Type>
SingleLinkedList<Type>::SingleLinkedList() = default;

template <typename Type>
SingleLinkedList<Type>::SingleLinkedList(std::initializer_list<Type> values) {
  Initialize(values.begin(), values.end(), values.size());
}

template <typename Type>
SingleLinkedList<Type>::SingleLinkedList(const SingleLinkedList& another) {
  assert(size_ == 0 && head_.next_node == nullptr);
  Initialize(another.begin(), another.end(), another.size_);
}

template <typename Type>
SingleLinkedList<Type>::~SingleLinkedList() {
  Clear();
}

template <typename Type>
SingleLinkedList<Type>& SingleLinkedList<Type>::operator=(
    const SingleLinkedList& rhs) {
  if (this == &rhs) return *this;
  Initialize(rhs.begin(), rhs.end(), rhs.size_);
  return *this;
}

template <typename Type>
void SingleLinkedList<Type>::swap(SingleLinkedList& another) noexcept {
  std::swap(head_.next_node, another.head_.next_node);
  std::swap(size_, another.size_);
}

template <typename Type>
[[nodiscard]] SingleLinkedList<Type>::BasicIterator<Type>
SingleLinkedList<Type>::begin() noexcept {
  return Iterator(head_.next_node);
}

template <typename Type>
[[nodiscard]] SingleLinkedList<Type>::BasicIterator<Type>
SingleLinkedList<Type>::end() noexcept {
  return Iterator(nullptr);
}
template <typename Type>
[[nodiscard]] SingleLinkedList<Type>::BasicIterator<const Type>
SingleLinkedList<Type>::begin() const noexcept {
  return ConstIterator(head_.next_node);
}

template <typename Type>
[[nodiscard]] SingleLinkedList<Type>::BasicIterator<const Type>
SingleLinkedList<Type>::end() const noexcept {
  return ConstIterator(nullptr);
}

template <typename Type>
[[nodiscard]] SingleLinkedList<Type>::BasicIterator<const Type>
SingleLinkedList<Type>::cbegin() const noexcept {
  return begin();
}
template <typename Type>
[[nodiscard]] SingleLinkedList<Type>::BasicIterator<const Type>
SingleLinkedList<Type>::cend() const noexcept {
  return end();
}

template <typename Type>
[[nodiscard]] SingleLinkedList<Type>::BasicIterator<Type>
SingleLinkedList<Type>::before_begin() noexcept {
  return Iterator(&head_);
}

template <typename Type>
[[nodiscard]] SingleLinkedList<Type>::BasicIterator<const Type>
SingleLinkedList<Type>::before_begin() const noexcept {
  return ConstIterator(const_cast<SingleLinkedList<Type>::Node*>(&head_));
}

template <typename Type>
[[nodiscard]] SingleLinkedList<Type>::BasicIterator<const Type>
SingleLinkedList<Type>::cbefore_begin() const noexcept {
  return before_begin();
}

template <typename Type>
[[nodiscard]] size_t SingleLinkedList<Type>::GetSize() const noexcept {
  return size_;
}

template <typename Type>
[[nodiscard]] bool SingleLinkedList<Type>::IsEmpty() const noexcept {
  return size_ == 0ull;
}

template <typename Type>
void SingleLinkedList<Type>::PushFront(const Type& value) {
  head_.next_node = new Node(value, head_.next_node);
  ++size_;
}

template <typename Type>
void SingleLinkedList<Type>::PopFront() noexcept {
  if (IsEmpty()) return;
  Node* head_next_ptr = head_.next_node;
  head_.next_node = head_.next_node->next_node;
  delete head_next_ptr;
  head_next_ptr = nullptr;
  --size_;
}

template <typename Type>
SingleLinkedList<Type>::BasicIterator<Type> SingleLinkedList<Type>::InsertAfter(
    SingleLinkedList<Type>::BasicIterator<const Type> pos, const Type& value) {
  assert(pos.node_ != nullptr);
  Node* new_node = new Node(value, pos.node_->next_node);
  pos.node_->next_node = new_node;
  ++size_;
  return Iterator(new_node);
}
template <typename Type>
SingleLinkedList<Type>::BasicIterator<Type> SingleLinkedList<Type>::EraseAfter(
    SingleLinkedList<Type>::BasicIterator<const Type> pos) noexcept {
  assert(pos.node_ != nullptr);
  Node* node_to_delete = pos.node_->next_node;
  pos.node_->next_node = (pos.node_->next_node)->next_node;
  delete node_to_delete;
  --size_;
  return Iterator(pos.node_->next_node);
}

template <typename Type>
void SingleLinkedList<Type>::Clear() noexcept {
  while (head_.next_node) {
    const Node* const front_node = head_.next_node;
    head_.next_node = head_.next_node->next_node;
    delete front_node;
  }
  size_ = 0ull;
}

template <typename Type>
template <typename T>
void SingleLinkedList<Type>::Initialize(T begin, T end, size_t size) {
  SingleLinkedList copy;
  auto copy_it = copy.before_begin();
  for (T it = begin; it != end; ++it) {
    copy.InsertAfter(copy_it, *it);
    ++copy_it;
  }

  copy.size_ = size;
  swap(copy);
}
// --end of single list methods--
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
  lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
  if (&lhs == &rhs) return true;
  if (lhs.GetSize() != rhs.GetSize()) return false;
  return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
  return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs,
               const SingleLinkedList<Type>& rhs) {
  return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(),
                                      rhs.cend());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
  return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs,
               const SingleLinkedList<Type>& rhs) {
  return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
  return !(lhs < rhs);
}
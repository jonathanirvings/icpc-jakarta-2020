import re
import string
import sys


class VerifierReader(object):
  def __init__(self, text):
    self.text = text
    self.position = 0

  def HasNext(self):
    return self.position < len(self.text)

  def Read(self, target):
    actual = self.text[self.position : self.position + len(target)]
    assert actual == target, "Could not read '{0}'".format(target)
    self.position += len(target)

  def ReadSpace(self):
    self.Read(' ')

  def ReadEndl(self):
    self.Read('\n')

  def ReadEof(self):
    assert self.position == len(self.text), 'Found junk at the end of the file'

  def ReadChar(self):
    assert self.position < len(self.text), 'Unexpected EOF'
    char = self.text[self.position]
    self.position += 1
    return char

  def ReadLine(self, valid_characters):
    old_position = self.position
    while True:
      assert self.position < len(self.text), 'Unexpected EOF'
      c = self.text[self.position]
      self.position += 1
      if c == '\n':
        break
      assert c in valid_characters
    assert self.position - old_position > 1, 'Nothing before next newline'
    return self.text[old_position:self.position - 1]

  def ReadRegex(self, target_regex):
    match = re.compile(target_regex).match(self.text, self.position)
    assert match is not None, 'Could not match /%s/' % target_regex
    self.position = match.end()
    return match.group(0)

  def ReadList(self, n, read_fn, *args):
    ls = []
    for i in range(n):
      if i > 0:
        self.Read(' ')
      ls.append(read_fn())
    self.ReadEndl()
    return ls

  def ReadInt(self, inclusive_min, inclusive_max):
    value = int(self.ReadRegex(r'0|(-?[1-9][0-9]*)'))
    assert inclusive_min <= value <= inclusive_max, (
        'Failed on %d <= %d <= %d' % (inclusive_min, value, inclusive_max))
    return value

  def ReadIntList(self, n, inclusive_min, inclusive_max):
    return self.ReadList(n, lambda: self.ReadInt(inclusive_min, inclusive_max))

  def ReadString(self, valid_characters=string.ascii_letters + string.digits,
                 inc_min_len=1, inc_max_len=10**6):
    old_position = self.position
    while True:
      assert self.position < len(self.text), 'Unexpected EOF'
      c = self.text[self.position]
      if c not in valid_characters:
        break
      self.position += 1
    st = self.text[old_position:self.position]
    assert inc_min_len <= len(st) <= inc_max_len, (
        'Invalid string length: %d' % len(st))
    return st

  def ReadStringList(self, n,
                     valid_characters=string.ascii_letters + string.digits,
                     inc_min_len=1, inc_max_len=10**6):
    return self.ReadList(
        n,
        lambda: self.ReadString(valid_characters, inc_min_len, inc_max_len))


def main(argv):
  vr = VerifierReader(sys.stdin.read())

  A, C, T = vr.ReadIntList(3, 1, 50000)

  union_L = {j:0 for j in range(1, C+1)}

  for _ in range(A):
    L = vr.ReadInt(0, 2)
    if L > 0:
      assert vr.ReadChar() == ' ', 'Input is not space separated'
      Lx = vr.ReadIntList(L, 1, C)
      assert len(Lx) == len(set(Lx)), 'L[x] is not unique'
      for j in Lx:
        union_L[j] += 1
    else:
      vr.ReadEndl()

  for j in range(1, C+1):
    assert union_L[j] <= 2, 'j appears more than twice in union_L'

  union_F = {k:0 for k in range(1, T+1)}

  for _ in range(A):
    F = vr.ReadInt(0, 2)
    if F > 0:
      assert vr.ReadChar() == ' ', 'Input is not space separated'    
      Fy = vr.ReadIntList(F, 1, T)
      assert len(Fy) == len(set(Fy)), 'F[y] is not unique'
      for k in Fy:
        union_F[k] += 1
    else:
      vr.ReadEndl()

  for k in range(1, T+1):
    assert union_F[k] <= 2, 'k appears more than twice in union_F'

  union_S = {k:0 for k in range(1, T+1)}

  for _ in range(C):
    S = vr.ReadInt(0, 2)
    if S > 0:
      assert vr.ReadChar() == ' ', 'Input is not space separated'    
      Sz = vr.ReadIntList(S, 1, T)
      assert len(Sz) == len(set(Sz)), 'S[z] is not unique'
      for k in Sz:
        union_S[k] += 1
    else:
      vr.ReadEndl()

  for k in range(1, T+1):
    assert union_S[k] <= 2, 'k appears more than twice in union_S'

  vr.ReadEof()


if __name__ == "__main__":
  sys.exit(main(sys.argv))

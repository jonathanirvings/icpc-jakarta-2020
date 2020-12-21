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


def IsClockwise(a, b, c):
  return (c[0] - b[0]) * (b[1] - a[1]) >= (c[1] - b[1]) * (b[0] - a[0])


def IsDegenerate(polygon):
  return sum(
      [
          polygon[i][0] * polygon[i + 1][1] - polygon[i][1] * polygon[i + 1][0]
          for i in range(len(polygon) - 1)
      ]) == 0


def main(argv):
  vr = VerifierReader(sys.stdin.read())

  N = vr.ReadInt(1, 500)
  assert vr.ReadChar() == ' ', 'Input is not space separated'
  M = vr.ReadInt(2, 500)
  assert vr.ReadChar() == ' ', 'Input is not space separated'
  S = vr.ReadInt(1, 99)
  vr.ReadEndl()

  military = [tuple(vr.ReadIntList(2, -10**9, 10**9)) for _ in range(N)]
  communication = [tuple(vr.ReadIntList(2, -10**9, 10**9)) for _ in range(M)]
  combined = military + communication

  assert len(combined) == len(set(combined)), 'Found duplicate tower position'

  communication = sorted(communication)
  chull = [communication[0]]
  for i in range(1, len(communication)):
    while len(chull) > 1 and not IsClockwise(chull[-2], chull[-1],
                                             communication[i]):
      chull.pop()
    chull.append(communication[i])
  limit = len(chull)
  for i in range(len(communication) - 2, -1, -1):
    while len(chull) > limit and not IsClockwise(chull[-2], chull[-1],
                                                 communication[i]):
      chull.pop()
    chull.append(communication[i])

  is_degenerate = IsDegenerate(chull)
  min_x_chull = min([point[0] for point in chull])
  max_x_chull = max([point[0] for point in chull])
  min_y_chull = min([point[1] for point in chull])
  max_y_chull = max([point[1] for point in chull])

  for i in range(len(military)):
    for j in range(len(chull) - 1):
      assert IsClockwise(chull[j], chull[j + 1], military[i]), (
          'Military base {} is initially not online'.format(i))
    if is_degenerate:
      assert (
          min_x_chull <= military[i][0] <= max_x_chull and
          min_y_chull <= military[i][1] <= max_y_chull), (
          'Military base {} is initially not online'.format(i))


  vr.ReadEof()


if __name__ == "__main__":
  sys.exit(main(sys.argv))

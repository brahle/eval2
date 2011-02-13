#!/usr/bin/env python3.1
from CommitMessage import CommitMessage

import os, re, sys

class ReviewBoardInvoker(object):
    """This class is used to invoke ReviewBoard tool post-review safely.
    TODO: example, more detailed description
    """
    def __init__(self, commit_message):
        self.commit_message = commit_message

    def write_to_file(self, file_name, text):
        f = open(file_name, 'w')
        f.write(text)
        f.close()

    def list_format(self, text):
        ret = ','.join(x.strip() for x in filter(None, text.split(',')))
        return ret

    def prepare(self):
        msg = self.commit_message.fields
        self.command = 'post-review --parent=HEAD^ -p'

        if 'title' in msg:
            self.command = self.command + ' --summary=' + str(msg['title'])
        if 'summary' in msg:
            file_name = '/tmp/eval2-RB-description'
            self.write_to_file(file_name, msg['summary'].get_value())
            self.command = self.command + ' --description-file=' + file_name
        if 'reviewers' in msg:
            self.command = (self.command + ' --target-people=' +
                            str(msg['reviewers']))
        if 'tickets' in msg:
            self.command = (self.command + ' --bugs-closed=' +
                            str(msg['tickets']))
        if 'reviewboardid' in msg:
            self.command = self.command + ' -r' + str(msg['reviewboardid'])

    def invoke(self):
        self.prepare()
        file_name = '/tmp/eval2-RB-result'
        self.command = self.command + ' >' + file_name
        print(self.command)

        if os.system(self.command) != 0:
            print("[WARNING] post-review failed!!!")
            print("See", file_name, "for details")
            self.reviewboard_id = -1
            return

        # Get the ReviewBoard ID from the message
        f = open(file_name, 'r')
        line = f.readline()
        pattern = re.compile('Review request #(?P<id>\d+) posted.')
        match = pattern.match(line)
        if match is None:
            print("[ABORT] post-review failed!!!")
            print("See ", file_name, " for details")
            sys.exit(1)
        self.reviewboard_id = int(match.group('id'))

    def get_reviewboard_id(self):
        return self.reviewboard_id

def main():
    message = """This is a sample message

              Summary: This is the summary of the sample message.
              This is a multi line one!
              Hoax field: just testing it.

              Tags: tag1, tag2

              Reviewers: brahle

              CCs: mark, lucas

              Blamed Revisions: r8, r12

              Tickets: 12341124, 2412341234151, 123412,
              """

    commit_message = CommitMessage(message)
    rbi = ReviewBoardInvoker(commit_message)
    rbi.invoke()


if __name__ == '__main__':
    main()


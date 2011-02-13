#!/usr/bin/env python3.1

from CommitMessage import CommitMessage
from ReviewBoardInvoker import ReviewBoardInvoker

import os, sys

def main():
    message_file = open(sys.argv[1], 'r')
    message = message_file.read()
    message_file.close()

    commit_message = CommitMessage(message)
    fields = commit_message.fields
    skip = True
    no = ['n', 'no', 'nay', 'ne']

    if 'skip reviewboard' in fields:
        value = fields['skip reviewboard'].get_value().lower()
        if value in no:
            skip = False

    if not skip:
        rbi = ReviewBoardInvoker(commit_message)
        print("Submitting data to ReviewBoard...")
        rbi.invoke()
        rb_id = rbi.get_reviewboard_id()
        if rb_id != -1:
            if 'reviewboardid' not in commit_message.fields:
                message_file = open(sys.argv[1], 'a')
                message_file.write('\nReviewBoardID: %d' % (rb_id))
            print("[SUCCESS] Submit done! ReviewBoardID: %d" % (rb_id))
            #TODO: This can be better
            print("http://evaluator.skole.hr:8080/reviews/r/%d/" % (rb_id))
    else:
        print("Skipping reviewboard! You better have a good reason for that!")

    print("\n[SUCCESS] Commit message survived!\n")

if __name__ == '__main__':
    main()


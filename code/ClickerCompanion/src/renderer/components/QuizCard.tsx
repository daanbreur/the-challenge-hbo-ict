/* eslint-disable jsx-a11y/no-static-element-interactions */
/* eslint-disable jsx-a11y/click-events-have-key-events */
import React from 'react';
import { IQuiz } from '../types';

type IProps = {
  quiz: IQuiz;
  setSelectedQuiz: React.Dispatch<React.SetStateAction<IQuiz>>;
  selected: boolean;
};

export default function QuizCard({ quiz, setSelectedQuiz, selected }: IProps) {
  return (
    <>
      <div className="p-2">
        <p className="font-normal text-lg">{quiz.title}</p>
      </div>
      <div className="p-2 ">
        <p className="font-normal text-base">{quiz.description}</p>
      </div>
      <div
        onClick={() => {
          setSelectedQuiz(quiz);
        }}
        className=" cursor-pointer"
      >
        Select kut Quiz {selected ? 'is selected a broer' : 'niet selected'}
      </div>
    </>
  );
}

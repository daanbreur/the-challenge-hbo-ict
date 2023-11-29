/* eslint-disable no-console */
/* eslint-disable no-return-assign */
/* eslint-disable jsx-a11y/no-static-element-interactions */
/* eslint-disable jsx-a11y/click-events-have-key-events */
import React, { useState } from 'react';
import PageWrapper from '../components/PageWrapper';
import QuizCard from '../components/QuizCard';
import logo from '../../../assets/logo.svg';

import { Props, IQuiz } from '../types';

import '../globals.css';
import '../fonts.css';
import 'tailwindcss/tailwind.css';

// eslint-disable-next-line import/no-cycle
import { getQuizzes } from '..';

export default function Overview({
  setCurrentPage,
  setSelectedQuiz,
  selectedQuiz,
}: Props) {
  const [loadedQuizzes, setLoadedQuizzes] = useState<IQuiz[]>([]);

  getQuizzes()
    .then((_quizzes) => setLoadedQuizzes(_quizzes))
    .catch((e) => console.error(e));

  return (
    <PageWrapper>
      <div className="flex ">
        <div className="sticky self-start top-0 w-[250px] h-[100vh] p-2 overflow-y-auto text-center bg-light_background">
          <div className="text-gray-100 text-xl">
            <div className="p-2.5 mt-1 flex items-center">
              <img draggable={false} className="" alt="icon" src={logo} />
              {/* <h1 className="font-bold text-text text-[15px] ml-3">
              TailwindCSS
            </h1> */}
            </div>
            <div className="my-2 bg-accent h-[1px]" />
          </div>
          <div className="p-2.5 mt-3 flex items-center rounded-md px-4 duration-300 cursor-pointer hover:bg-blue-600 text-white">
            <span className="text-[15px] ml-4 text-text font-bold">Home</span>
          </div>
          <div className="p-2.5 mt-3 flex items-center rounded-md px-4 duration-300 cursor-pointer hover:bg-blue-600 text-white">
            <span className="text-[15px] ml-4 text-text font-bold">
              Bookmark
            </span>
          </div>
          <div className="my-4 bg-accent h-[1px]" />
          <div className="p-2.5 mt-3 flex items-center rounded-md px-4 duration-300 cursor-pointer hover:bg-blue-600 text-white">
            <span className="text-[15px] ml-4 text-text font-bold">Logout</span>
          </div>
        </div>

        <div className="flex-1 h-[100h] font-bold text-3xl">
          {loadedQuizzes.map((quiz: IQuiz) => (
            <QuizCard
              quiz={quiz}
              setSelectedQuiz={setSelectedQuiz}
              selected={selectedQuiz?.id === quiz.id}
            />
          ))}
        </div>
      </div>
    </PageWrapper>
  );
}

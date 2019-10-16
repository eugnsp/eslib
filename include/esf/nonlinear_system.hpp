#pragma once
#include "es_la/dense.hpp"
#include "es_la/dense_function.hpp"
#include "es_la/sparse.hpp"
#include "system.hpp"
// #include "math/LA/VectorX.h"
// #include "math/LA/VectorFunction.h"
// #include "math/LA/SparseMatrix.h"
// #include "util/log/log.hpp"
#include <cstddef>
#include <string>

#include <iomanip>
#include <iostream>

namespace esf
{
template<class Linear_solver, class Var_or_var_list,
	template<class> class T_Dof_mapper = Dof_mapper>
class Nonlinear_system : public System<Var_or_var_list, T_Dof_mapper>
{
private:
	using Base = System<Var_or_var_list, T_Dof_mapper>;

public:
	using Base::Base;

	virtual std::size_t memory_size() const override
	{
		// TODO : capacity(), not size()
		return Base::memory_size() + jacobian_.memory_size() // +
			   + (residual_.size() + step_.size()) * sizeof(double);
	}

protected:
	virtual void assemble() = 0;

	virtual void before_solve()
	{}

	virtual void after_solve()
	{}

	virtual void after_step(const esl::Vector_xd&)
	{}

	virtual void generate_init_guess() = 0;

	virtual void set_bnd_values()
	{}

	virtual double residual_norm2()
	{
		return norm2(residual_);
	}

private:
	// 	virtual void init_storage() override
	// 	{
	// 		const auto n = this->n_free_dofs();
	// 		jacobian_.resize(n, n);
	// 		residual_.resize(n);
	// 		step_.resize(n);
	// 	}

	virtual void do_solve() override final
	{
		before_solve();

		generate_init_guess();
		set_bnd_values();

		linear_solver_.analyze(jacobian_);

		const double armijoConstant = 1e-4;
		double residual_norm_old = 0;
		double reduction_factor = 1;

		// 		LOG << "-------------------------------------------------------\n"
		// 			<< "N         |residual|      |step|          LS damping\n"
		// 			<< "-------------------------------------------------------" << std::endl;

		std::size_t iteration = 0;
		for (;;)
		{
			assemble();

#ifdef check1
			const auto dphi = 1e-7;
			auto r1 = residual_;

			// for (Index i = 0; i < this->n_free_dofs(); i += 100)
			//{

			auto i = 4000;
			solution_[i] += dphi;
			assemble();
			solution_[i] -= dphi;

			// 				for (Index j = 0; j < this->n_free_dofs(); ++j)
			// 				{
			for (auto j : {3987, 3664, 4000})
			{
				auto J = jacobian_.element(i, j);
				// 					if (J == 0)
				// 						continue;

				auto dr = residual_ - r1;
				auto Jp = dr[j] / dphi;

				// 					if (std::abs((J - Jp)) > 1e-5 * std::abs(J + Jp))
				// 					{
				std::cout << i << ", " << j << ": " << J << ", " << Jp << '\n';
				// 					}
			}
			// std::cout << i << ' ';
			//		}
#endif

			const double residual_norm = residual_norm2();

			if (iteration > 10 &&
				residual_norm > residual_norm_old * (1 - armijoConstant * reduction_factor))
			{
				reduction_factor /= 2;
				step_ /= 2;
				solution_.view(0, 0, step_.size(), 1) += step_;

				std::cout << std::setprecision(3) << std::scientific << std::setw(3)
						  << iteration + 1 << std::setw(16) << residual_norm << std::setw(32)
						  << reduction_factor << std::endl;

				++iteration;
				continue;
			}

			reduction_factor = 1;
			residual_norm_old = residual_norm;

			linear_solver_.factorize_solve(jacobian_, residual_, step_);

			const double step_norm = norm2(step_);

			std::cout << std::setprecision(3) << std::scientific << std::setw(3) << iteration + 1
					  << std::setw(16) << residual_norm << std::setw(16) << step_norm << std::endl;

			if (step_norm < 1e-8)
			{
				break;
			}

			solution_.view(0, step_.size(), 0, 1) -= step_;
			++iteration;

			after_step(step_);
		}

		after_solve();
	}

protected:
	using Base::solution_;

	typename Linear_solver::Sparse_matrix jacobian_;
	esl::Vector_xd residual_;
	Linear_solver linear_solver_;

private:
	esl::Vector_xd step_;
};

//		template<std::size_t _dim>
//		class NonlinearSystem : public System2<_dim>
//		{
//		private:
//			using Base = System2<_dim>;
//
//		public:
//			using Base::Base;
//
//			void init()
//			{
//				Base::init();
//
//				residual_.resize(dofMap_.n_free_dofs());
//				step_.resize(dofMap_.n_free_dofs());
//			}
//
//		protected:
//			virtual void doSolve() override
//			{
//				assert(linearSolver_);
//
//				linearSolver_->Init(*jacobian_);
//
//				std::size_t numberOfIterations = 0;
//				for (;;)
//				{
//					assembleRJ();
//					const double residualNorm = residualNorm2();
//
//					linearSolver_->Solve(*jacobian_, residual_, step_);
//
//					const double linearStepSupNorm = normSup(step_);
//
//					LOG(LogLevel::INFO, "%4u                %11.3e         %11.3e",
// numberOfIterations
//+ 1, residualNorm, linearStepSupNorm);
//
//					if (numberOfIterations > 0 && linearStepSupNorm < 1e-10)
//					{
//						break;
//					}
//
//					solution_.view(step_.size()) -= step_;
//					++numberOfIterations;
//				}
//
//				linearSolver_->Clear();
//
//				afterSolve();
//			}
//
//			virtual void assembleRJ() = 0;
//
//			virtual double residualNorm2()
//			{
//				return norm2(residual_);
//			}
//
//			virtual void afterSolve() const = 0;
//
//		protected:
//			std::unique_ptr<Math::LA::SparseMatrix<double>> jacobian_;
//			Math::LA::VectorX<double> residual_;
//			Math::LA::VectorX<double> step_;
//		};
//
//
//		template<std::size_t dim, class DOFMapType = DOFMap<dim>>
//		class NonlinSystem : public System<dim, DOFMapType>
//		{
//		public:
//// 			class FeNonlinSysNewtonStatus : public SolverStatus
//// 			{
//// 				template<class, typename> friend class NonlinSystem;
////
//		public:
//			enum class Status
//			{
//				OK,
//				UNKNOWN,
//				TOO_MANY_ITERATIONS_FAIL,
//				LINE_SEARCH_FAIL,
//				LINEAR_SOLVER_FAIL,
//				NAN_FAIL
//			};
//
//		public:
//			bool isOK() const
//			{
//				return status_ == Status::OK;
//			}
//
//				//void SetStatus(Status s) { status = s; }
//// 				void SetNumberOfIterations(unsigned int i) { numberOfIterations = i; }
////
//// 			public:
//// 				Status GetStatus() const { return status; }
//// 				virtual std::string GetStatusString() const override;
////
//// 				virtual bool IsOK() const override { return status == Status::OK; }
//// 			};
//
//		private:
//			virtual bool doSolve() override;
//
//			//			typedef LinSystem<MESH, T> Parent;
//
//			/** Solving */
//// 			virtual bool DoSolve() override final;
////
//// 			void CheckJacobianByFD();
//
//		protected:
////			FeNonlinSysNewtonStatus status;
//
//// 			std::unique_ptr<Math::LA::SparseMatrix<T>> jacobian;
//// 			Math::LA::VectorX<T> residual;
//// 			Math::LA::VectorX<T> linearStep;
////
//// 			std::unique_ptr<Math::LA::SparseSolver<T>> linearSolver;
//
//			/** Parameters */
//			const double backtrackFactor = .5;
//
//// 			virtual void BeforeSolve() { }
////
// 			virtual void assembleRJ() = 0;
//
//			virtual double residualNorm2()
//			{
//				return norm2(residual_);
//			}
//
//			virtual void afterSolve() { }
//
//// 			virtual void AfterStep(unsigned int) { }
//// 			virtual void AfterLineSearchFail(unsigned int) { }
////
//			//virtual void AssembleResidual() { }
////
//			using System::System;
//
//			/** Initializes the system. */
//			void init()
//			{
//				System::init();
//
//				residual_.resize(dofMap_.n_free_dofs());
//				linearStep_.resize(dofMap_.n_free_dofs());
//			}
//
//			/** Initialization */
//// 			virtual void Init() override;
////
//// 			/** Status */
//// 			FeNonlinSysNewtonStatus GetStatus() const { return status; }
//
//		private:
//			void checkJacobianByFD();
//
//		protected:
//			LA::VectorX<double> residual_;
//
//			LA::VectorX<double> linearStep_;
//
//			std::unique_ptr<LA::SparseMatrix<double>> jacobian_;
//
//		private:
//			Status status_ = Status::UNKNOWN;
//			unsigned int numberOfIterations_;
//		};
//
//		template<std::size_t dim, class DOFMapType>
//		bool NonlinSystem<dim, DOFMapType>::doSolve()
//		{
//			assert(linearSolver_);
//
//			status_ = Status::UNKNOWN;
//			PERF_LOG_START(__FUNCTION__);
//
//			double absoluteStepTolerance = phys::Unit::from_eV(1e-5);
//			unsigned int maxNumberOfIterations = 700;
//			const double minReductionFactor = .005;
//			const double armijoConstant = 1e-4;
//
//			//		 	status.SetStatus(FeNonlinSysNewtonStatus::Status::NOT_SOLVED);
//			//
//			// 	LOG_INFO("Nonlinear solver: Exact Newton with back-tracking line search");
//			// 	LOG_INFO("Linear solver: " + linearSolver->GetDisplayName());
//			// 	LOG_INFO("------------------------------------------------------");
//			// 	LOG_INFO("Iter     Damping     Residual 2-norm     Step sup-norm");
//			// 	LOG_INFO("------------------------------------------------------");
//
//			double residualNormOld;
//			double linearStepSupNorm;
//
//			numberOfIterations_ = 0;
//			unsigned int lineSearchIter = 0;
//			double reductionFactor = 1;
//
//			//{
//			//	IO::MATFileOutput m("p0.mat", mesh_, phys::Unit::from_nm(1));
//			//	m.writeDataOnCells("phi", solutionView("phi"), phys::Unit::from_eV(1));
//			//}
//
//			// 	//#define PUT0
//			// 	#ifdef PUT0
//			// 	{
//			// 		FeMATFileOutput<MESH> mo("pc0.mat", mesh, Length::nm(1));
//			// 		if (hasVariable("phi")) mo.PutScalarDataOnNodes("phi", GetSolutionView("phi"),
// Energy::eV(1));
//			// 		if (hasVariable("fermi")) mo.PutScalarDataOnNodes("fermi",
// GetSolutionView("fermi"), Energy::eV(1));
//			// 		if (hasVariable("n")) mo.PutScalarDataOnNodes("n", GetSolutionView("n"),
// Concentration::ONE_OVER_cm3(1));
//			// 	}
//			// 	#endif
//			//
//			// 	//for (unsigned int i = residual.GetSize() / 2; i < residual.GetSize(); ++i)
//			// 	//	solution[i] += Energy::eV(.1);
//			//
//			// 	//solution[dofMap.GetDOFIndex(1217, 1)] += Energy::eV(.15);
//			// //	solution[dofMap.GetDOFIndex(1217, 1)] += Energy::eV(.15);
//			// //	solution[dofMap.GetDOFIndex(1218, 1)] += Energy::eV(.15);
//
//			// Initialize sparse solver (sparsity pattern of Jacobian is known at this point)
//			linearSolver_->Init(*jacobian_);
//
//			// 	//VectorX<double> dx(residual.GetSize());
//			//
//			// 	//AssembleResidualAndJacobian();
//			// 	//VectorX<double> res1(residual);
//			//
//			// 	//dx.zero();
//			// 	//dx[12000] = 1e-8;
//			// 	//solution.AddSubvectorAtPos(dx, 0);
//			//
//			// 	//AssembleResidualAndJacobian();
//			// 	//VectorX<double> res2(residual);
//			//
//			// 	//VectorX<double> dres = res2 - res1;
//			// 	//VectorX<double> Jdx = (*jacobian) * dx;
//			//
//			// 	//Jdx -= dres;
//			// 	//std::cout << Jdx.GetNorm2() << '\n';
//			//
//			// 	BeforeSolve();
//			//
//			// 	bool zzz;
//			//
//			for (;;)
//			{
//				assembleRJ();
//				const double residualNorm = residualNorm2();
//
////				checkJacobianByFD();
//
//				// 		//#define PUT_J
//				// 		#ifdef PUT_J
//				// 		{
//				// 			MATFile m("j.mat");
//				// 			m.write("J",
// dynamic_cast<Math::LA::CSRSparseMatrix<double>&>(*jacobian));
//				// 		}
//				// 		#endif
//				//
//				// 		// Jacobian check
//				// 		//CheckJacobianByFD();
//				// 		//return false;
//				//
//				if (std::isnan(residualNorm))
//				{
//					status_ = Status::NAN_FAIL;
//					break;
//				}
//
//				// Armijo line-search
//				// 		if (numberOfIterations > 0 && residualNorm > residualNormOld * (1 -
// armijoConstant
//* reductionFactor))
//				// 		{
//				// 			//AfterLineSearchFail(lineSearchIter);
//				//
//				// 			// Backtracking
//				// 			if (reductionFactor * backtrackFactor < minReductionFactor)
//				// 			{
//				// 				status.SetStatus(FeNonlinSysNewtonStatus::Status::LINE_SEARCH_FAIL);
//				//
//				// 				AfterLineSearchFail(lineSearchIter);
//				//
//				// 				//continue;
//				// 				goto skip;
//				// 				break;
//				// 			}
//				//
//				// 			reductionFactor *= backtrackFactor;
//				// 			linearStep *= backtrackFactor;
//				//
//				// 			solution.view(linearStep.size()) += linearStep;
//				//
//				// 			++lineSearchIter;
//				// 			LOG_INFO(" * %3u   %6.4f     %11.3e", lineSearchIter, reductionFactor,
// residualNorm);
//				//
//				// 			continue;
//				// 		}
//				//
//				// 	skip:
//				//
//				if (numberOfIterations_ >= maxNumberOfIterations)
//				{
//					status_ = Status::TOO_MANY_ITERATIONS_FAIL;
//					break;
//				}
//
//				lineSearchIter = 0;
//				reductionFactor = 1;
//				residualNormOld = residualNorm;
//
//				// 		//if (iter >= maxNumberOfIterations)
//				// 		//{
//				// 		//	// Set error flag and exit
//				// 		//	break;
//				// 		//}
//				//
//				// 		for (std::size_t i = 0; i < residual.size(); ++i)
//				// 		{
//				// 			if (std::isnan(residual[i]))
//				// 			{
//				// 				LOG_ERR("R NaN at: %d", i);
//				// 				break;
//				// 			}
//				// 		}
//				//
//				// 		auto& J = static_cast<Math::LA::CSRSparseMatrix<double>&>(*jacobian);
//				// 		for (std::size_t i = 0; i < J.nnz(); ++i)
//				// 		{
//				// 			auto el = J.valuesData();
//				// 			if (std::isnan(el[i]))
//				// 			{
//				// 				LOG_ERR("J NaN at: %d", i);
//				// 				break;
//				// 			}
//				// 		}
//				//
//				// 		//		sparseSolver->Rescale(*jacobian, residual);
//				if (!linearSolver_->Solve(*jacobian_, residual_, linearStep_))
//				{
//					status_ = Status::LINEAR_SOLVER_FAIL;
//					break;
//				}
//
//				linearStepSupNorm = normSup(linearStep_);
//
//				LOG(LogLevel::INFO, "%4u                %11.3e         %11.3e", numberOfIterations_
//+ 1, residualNorm, linearStepSupNorm);
//
//				if (std::isnan(linearStepSupNorm))
//				{
//					status_ = Status::NAN_FAIL;
//					break;
//				}
//
//				if (numberOfIterations_ > 0 && linearStepSupNorm < absoluteStepTolerance)
//				{
//					status_ = Status::OK;
//					break;
//				}
//
//				// 		//step *= .02;
//				// 		//std::vector<double> res;
//				// 		//for (unsigned int i = 0; i < 50; ++i)
//				// 		//{
//				// 		//	solution.SubtractSubvectorAtPos(step, 0);
//				// 		//	GetJacobianAndResidual(*jacobian, residual);
//				// 		//	res.push_back(residual.GetNorm2());
//				// 		//}
//				//
//				// 		//MatFile m("res" + std::to_string(iter) + ".mat");
//				// 		//m.putVar("res", res);
//				//
//				// 		// Make full Newton step first
//				// 		//linearStep /= 5;
//				solution_.view(linearStep_.size()) -= linearStep_;
//
//				// 		//if (GetNumberOfVariables() == 2)
//				// 		//{
//				// 		//	MATFile o("j.mat");
//				// 		//	o.putVar("j", static_cast<CSRSparseMatrix<double>&>(*jacobian));
//				// 		////	o.putVar("r", residual);
//				// 		////	o.putVar("x", linearStep);
//				// 		//}
//				//
//				// 		#define WRITE_SOLUTION
//				// 		#if defined(WRITE_SOLUTION) && defined(_DEBUG)
//				// 		if (numberOfVariables() == 2)
//				// 		{
//				// 			//const Math::LA::VectorX<double> temp(solution);
//				//
//				// 			////AssembleResidualAndJacobian(-1);
//				//
//				// 			//{
//				// 			//	FeMATFileOutput<Math::mesh::Mesh2> mo("system.mat", mesh,
// Length::nm(1));
//				// 			//	mo.PutScalarDataOnNodes("phi", GetSolutionView("phi"));
//				// 			//	//mo.PutScalarDataOnNodes("psi", GetSolutionView("psi"),
// Energy::eV(1));
//				// 			//	//mo.PutScalarDataOnNodes("fermi", GetSolutionView("psi") -
// GetSolutionView("phi"), Energy::eV(1));
//				// 			//	//mo.PutScalarDataOnNodes("fermi", GetSolutionView("fermi"),
// Energy::eV(1));
//				// 			//	if (HasVariable("n")) mo.PutScalarDataOnNodes("n",
// GetSolutionView("n"));
//				// 			//	if (HasVariable("F")) mo.PutScalarDataOnNodes("F",
// GetSolutionView("F"));
//				// 			//	//mo.GetMATFile().PutVar("J",
// dynamic_cast<CSRSparseMatrix<double>&>(*jacobian));
//				//
//				// 			//	solution.zero();
//				// 			//	solution.GetView(residual.GetSize()) = residual;
//				//
//				// 			//	mo.PutScalarDataOnNodes("r_phi", GetSolutionView("phi"));
//				// 			//	//mo.PutScalarDataOnNodes("r_psi", GetSolutionView("psi"),
// Energy::eV(1));
//				// 			//	if (HasVariable("n")) mo.PutScalarDataOnNodes("r_n",
// GetSolutionView("n"));
//				//
//				// 			//	solution.zero();
//				// 			//	solution.GetView(linearStep.GetSize()) = linearStep;
//				//
//				// 			//	mo.PutScalarDataOnNodes("d_phi", GetSolutionView("phi"));
//				// 			//	//mo.PutScalarDataOnNodes("d_psi", GetSolutionView("psi"),
// Energy::eV(1));
//				// 			//	//mo.PutScalarDataOnNodes("d_fermi", GetSolutionView("psi") -
// GetSolutionView("phi"), Energy::eV(1));
//				// 			//	//mo.PutScalarDataOnNodes("d_fermi", GetSolutionView("fermi"),
// Energy::eV(1));
//				// 			//	if (HasVariable("n")) mo.PutScalarDataOnNodes("d_n",
// GetSolutionView("n"));
//				// 			//	if (HasVariable("F")) mo.PutScalarDataOnNodes("d_F",
// GetSolutionView("F"));
//				// 			//}
//				//
//				// 			//solution = temp;
//				//
//				// 			//std::cout << solution[dofMap.GetDOFIndex(827, 0)] << ' ' <<
// solution[dofMap.GetDOFIndex(827, 1)] << '\n';
//				// 			//std::cout << solution[dofMap.GetDOFIndex(776, 0)] << ' ' <<
// solution[dofMap.GetDOFIndex(776, 1)] << '\n' << "----------------\n";
//				// 			//std::cout << residual[dofMap.GetDOFIndex(827, 0)] << ' ' <<
// residual[dofMap.GetDOFIndex(827, 1)] << '\n';
//				// 			//std::cout << residual[dofMap.GetDOFIndex(776, 0)] << ' ' <<
// residual[dofMap.GetDOFIndex(776, 1)] << '\n';
//				// 		}
//				// 		#endif
//				//
//				// 		//if (numberOfIterations > 0)
//				// 		//{
//				// 		//	solution.GetView(linearStep.GetSize()) += linearStep; // restore
//				//
//				// 		//	for (unsigned int i = 0; i < 30; ++i)
//				// 		//	{
//				// 		//		solution.GetView(linearStep.GetSize()) -= (i / 12.) * linearStep;
//				// 		//		AssembleResidualAndJacobian();
//				// 		//		std::cout << residual[dofMap.GetDOFIndex(827, 0)] << " | " <<
// residual[dofMap.GetDOFIndex(827, 1)] << " | " <<
//				// 		//				     residual[dofMap.GetDOFIndex(776, 0)] << " | " <<
// residual[dofMap.GetDOFIndex(776, 1)] << '\n';
//				// 		//	}
//				// 		//}
//				//
//				// 		if (numberOfVariables() == 2)
//				// 		{
//				// 			for (std::size_t i = solution.size() / 2; i < solution.size(); ++i)
//				// 			{
//				// 				if (solution[i] < 0)
//				// 					solution[i] += linearStep[i];
//				// 			}
//				// 		}
//				//
//				++numberOfIterations_;
//				//AfterStep(numberOfIterations);
//			}
//			//
//			// 	LOG_INFO("------------------------------------------------------");
//			//
//			// 	if (status.IsOK())
//			// 		LOG_INFO(status.GetStatusString());
//			// 	else
//			// 		LOG_ERR(status.GetStatusString());
//
//			linearSolver_->Clear();
//
//			PERF_LOG_STOP(__FUNCTION__);
//
//			afterSolve();
//			return isOK();
//		}
//
//		template<std::size_t dim, class DOFMapType>
//		void NonlinSystem<dim, DOFMapType>::checkJacobianByFD()
//		 {
//		 	LOG_INFO("*************************************************************");
//		 	LOG_INFO("Check Jacobian matrix using finite differences");
//		 	LOG_INFO("-------------------------------------------------------------");
//		 	LOG_INFO("Matrix element     Finite diff.    Exact deriv.    Rel. error");
//		 	LOG_INFO("-------------------------------------------------------------");
//
//		 	const double dd = 1e-6;
//
//		 	assembleRJ();
//
//		 	const auto J0 = dynamic_cast<Math::LA::CSRSparseMatrix<double>&>(*jacobian_);
//		 	const auto r0 = residual_;
//
//		 	const auto size = residual_.size();
//		 	const auto startCol = 0; // residual.GetSize() / 2;
//
//		 	for (unsigned int col = startCol; col < size; ++col)
//		 	{
//		 		auto oldSolution = solution_;
//		 		solution_[col] += dd;
//
//				assembleRJ();
//		 		Math::LA::VectorX<double> dr = residual_ - r0;
//		 		dr /= dd;
//
//		 		Math::LA::VectorX<double> jc(size, 0);
//		 		jc[col] = 1;
//		 		jc = J0 * jc;
//
//		 		for (unsigned int row = 0; row < size; ++row)
//		 			if (jc[row] != 0 || dr[row] != 0)
//		 				LOG_INFO("(%4u, %4u)     %11.3e     %11.3e     %9.1e", row, col, dr[row],
// jc[row], 		 					std::abs(dr[row] - jc[row]) / std::max(std::abs(dr[row]),
// std::abs(jc[row])));
//
//		 		solution_ = oldSolution;
//		 	}
//		 }
} // namespace esf

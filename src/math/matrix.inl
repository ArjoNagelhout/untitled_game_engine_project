//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef BORED_ENGINE_MATRIX_INL
#define BORED_ENGINE_MATRIX_INL

#include "matrix.h"

namespace math
{
	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr float& Matrix<Rows, Columns>::operator()(matrix_size_type row, matrix_size_type column)
	{
		return _data[row][column];
	}



	template<matrix_size_type Rows, matrix_size_type Columns>
	template<matrix_size_type rhsRows, matrix_size_type rhsColumns>
	constexpr Matrix<Rows, rhsColumns> Matrix<Rows, Columns>::operator*(Matrix<rhsRows, rhsColumns> const& rhs) requires (Columns == rhsRows)
	{
		Matrix<Rows, Columns> result{};
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < rhsColumns; j++)
			{
				for (int k = 0; k < Columns; k++)
				{
					result._data[i][j] += _data[i][k] * rhs._data[k][j];
				}
			}
		}
		return result;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	[[nodiscard]] std::string Matrix<Rows, Columns>::toString() const
	{
		std::stringstream result{};
		result << "{";
		for (int i = 0; i < Rows; i++)
		{
			if (i > 0)
			{
				result << " ";
			}
			for (int j = 0; j < Columns; j++)
			{
				result << _data[i][j];
				if (j < Columns - 1)
				{
					result << ", ";
				}
			}
			if (i < Rows - 1)
			{
				result << ", \n";
			}
		}
		result << "}";
		return result.str();
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Rows, Columns> Matrix<Rows, Columns>::createIdentity() requires (Rows == Columns)
	{
		Matrix<Rows, Columns> result{};
		for (int i = 0; i < Rows; i++)
		{
			result(i, i) = 1.f;
		}
		return result;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Rows, Columns> Matrix<Rows, Columns>::identity = Matrix<Rows, Columns>::createIdentity();

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Rows, Columns> Matrix<Rows, Columns>::zero = Matrix<Rows, Columns>{};
}

#endif //BORED_ENGINE_MATRIX_INL

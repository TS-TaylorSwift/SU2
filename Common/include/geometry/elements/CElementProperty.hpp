﻿/*!
 * \file CElementProperty.hpp
 * \brief Light classes to define finite element properties.
 * \author R. Sanchez
 * \version 6.2.0 "Falcon"
 *
 * The current SU2 release has been coordinated by the
 * SU2 International Developers Society <www.su2devsociety.org>
 * with selected contributions from the open-source community.
 *
 * The main research teams contributing to the current release are:
 *  - Prof. Juan J. Alonso's group at Stanford University.
 *  - Prof. Piero Colonna's group at Delft University of Technology.
 *  - Prof. Nicolas R. Gauger's group at Kaiserslautern University of Technology.
 *  - Prof. Alberto Guardone's group at Polytechnic University of Milan.
 *  - Prof. Rafael Palacios' group at Imperial College London.
 *  - Prof. Vincent Terrapon's group at the University of Liege.
 *  - Prof. Edwin van der Weide's group at the University of Twente.
 *  - Lab. of New Concepts in Aeronautics at Tech. Institute of Aeronautics.
 *
 * Copyright 2012-2019, Francisco D. Palacios, Thomas D. Economon,
 *                      Tim Albring, and the SU2 contributors.
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

/*!
 * \class CProperty
 * \brief Base class for defining element properties.
 * \author R. Sanchez
 * \version 6.2.0 "Falcon"
 */
class CProperty {
protected:

  unsigned long iMat_Prop = 0;  /*!< \brief Index of the properties (E, Nu) for the structural model used. */

public:
  /*!
   * \brief Constructor of the class.
   * \param[in] valMat_Prop - Index of the physical properties (E,nu,rho,rho_dead_load) assigned to the element.
   */
  CProperty(unsigned long valMat_Prop) : iMat_Prop(valMat_Prop) {}

  /*!
   * \brief Destructor of the class.
   */
  virtual ~CProperty(void) = default;

  /*!
   * \brief Get the material model to use for the element.
   */
  inline virtual unsigned long GetMat_Mod(void) const { return 0; } 

  /*!
   * \brief Get index of the physical properties.
   */
  inline unsigned long GetMat_Prop(void) const { return iMat_Prop; }

  /*!
   * \brief Get index of the electric properties.
   */
  inline virtual unsigned long GetElectric_Prop(void) const { return 0; }

  /*!
   * \brief Get index of the design variable.
   */
  inline virtual unsigned long GetDV(void) const { return 0; }

  /*!
   * \brief Set the Design density (topology optimization variable).
   */
  inline virtual void SetDesignDensity(su2double valDensity) {}

  /*!
   * \brief Get the value of the Design density.
   */
  inline virtual su2double GetDesignDensity(void) const { return 0.0; }

  /*!
   * \brief Set the Physical density (used to penalize element stiffness by the FEM solver).
   */
  inline virtual void SetPhysicalDensity(su2double valDensity) {}

  /*!
   * \brief Get the value of the Physical density.
   */
  inline virtual su2double GetPhysicalDensity(void) const { return 0.0; }

  /*!
   * \brief Extract the derivative of the Design density.
   */
  inline virtual su2double GetAdjointDensity(void) const { return 0.0; }

  /*!
   * \brief Register the Design density as an AD input variable.
   */
  inline virtual void RegisterDensity(void) {}
};


/*!
 * \class CElementProperty
 * \brief Class for defining element properties for the structural solver.
 * \author R. Sanchez
 * \version 6.2.0 "Falcon"
 */
class CElementProperty final : public CProperty {
private:

  unsigned long iMat_Mod = 0;        /*!< \brief Index of the material model used. */
  unsigned long iElectric_Prop = 0;  /*!< \brief Index of the electric properties (Em) for the structural model used. */
  unsigned long iDV = 0;             /*!< \brief Index of the group of design variables to which the element belongs. */
  su2double design_rho = 1.0;        /*!< \brief Value of the design density for material-based topology optimization. */
  su2double physical_rho = 1.0;      /*!< \brief Value of the physical density for material-based topology optimization. */

public:

  /*!
   * \brief Constructor of the class.
   * \param[in] valMat_Model - Type of material model (i.e. numerics) for the element, see FEA_TERM etc. in option_structure.hpp.
   * \param[in] valMat_Prop - Index of the physical properties (E,nu,rho,rho_dead_load) assigned to the element.
   * \param[in] valElectric_Prop - Index of the electric properties.
   * \param[in] valDV - Index of the design variable assigned to the element (bound to a material property by "DESIGN_VARIABLE_FEA").
   * \param[in] valDensity - Value for Design and Physical densities (topology optimization variables).
   */
  CElementProperty(unsigned long valMat_Model, unsigned long valMat_Prop,
                   unsigned long valElectric_Prop, unsigned long valDV,
                   su2double valDensity = 1.0) : CProperty(valMat_Prop),
    iMat_Mod(valMat_Model), iElectric_Prop(valElectric_Prop),
    iDV(valDV), design_rho(valDensity), physical_rho(valDensity) {}

  /*!
   * \brief Destructor of the class.
   */
  ~CElementProperty(void) = default;

  /*!
   * \brief Get the material model to use for the element.
   */
  inline unsigned long GetMat_Mod(void) const override { return iMat_Mod; }

  /*!
   * \brief Get index of the electric properties.
   */
  inline unsigned long GetElectric_Prop(void) const override { return iElectric_Prop; }

  /*!
   * \brief Get index of the design variable.
   */
  inline unsigned long GetDV(void) const override { return iDV; }
  
  /*!
   * \brief Set the Design density (topology optimization variable).
   */
  inline void SetDesignDensity(su2double valDensity) override { design_rho = valDensity; }
  
  /*!
   * \brief Get the value of the Design density.
   */
  inline su2double GetDesignDensity(void) const override { return design_rho; }
  
  /*!
   * \brief Set the Physical density (used to penalize element stiffness by the FEM solver).
   */
  inline void SetPhysicalDensity(su2double valDensity) override { physical_rho = valDensity; }
  
  /*!
   * \brief Get the value of the Physical density.
   */
  inline su2double GetPhysicalDensity(void) const override { return physical_rho; }
  
  /*!
   * \brief Extract the derivative of the Design density.
   */
  inline su2double GetAdjointDensity(void) const override { return SU2_TYPE::GetDerivative(design_rho); }
  
  /*!
   * \brief Register the Design density as an AD input variable.
   */
  inline void RegisterDensity(void) override { AD::RegisterInput(design_rho); }
};
